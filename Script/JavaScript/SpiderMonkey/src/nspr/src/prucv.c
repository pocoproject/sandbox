/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 2 -*- */
/* ***** BEGIN LICENSE BLOCK *****
 * Version: MPL 1.1/GPL 2.0/LGPL 2.1
 *
 * The contents of this file are subject to the Mozilla Public License Version
 * 1.1 (the "License"); you may not use this file except in compliance with
 * the License. You may obtain a copy of the License at
 * http://www.mozilla.org/MPL/
 *
 * Software distributed under the License is distributed on an "AS IS" basis,
 * WITHOUT WARRANTY OF ANY KIND, either express or implied. See the License
 * for the specific language governing rights and limitations under the
 * License.
 *
 * The Original Code is the Netscape Portable Runtime (NSPR).
 *
 * The Initial Developer of the Original Code is
 * Netscape Communications Corporation.
 * Portions created by the Initial Developer are Copyright (C) 1998-2000
 * the Initial Developer. All Rights Reserved.
 *
 * Contributor(s):
 *
 * Alternatively, the contents of this file may be used under the terms of
 * either the GNU General Public License Version 2 or later (the "GPL"), or
 * the GNU Lesser General Public License Version 2.1 or later (the "LGPL"),
 * in which case the provisions of the GPL or the LGPL are applicable instead
 * of those above. If you wish to allow use of your version of this file only
 * under the terms of either the GPL or the LGPL, and not to allow others to
 * use your version of this file under the terms of the MPL, indicate your
 * decision by deleting the provisions above and replace them with the notice
 * and other provisions required by the GPL or the LGPL. If you do not delete
 * the provisions above, a recipient may use your version of this file under
 * the terms of any one of the MPL, the GPL or the LGPL.
 *
 * ***** END LICENSE BLOCK ***** */



#include "prinrval.h"
#include "prtypes.h"
#include "prcvar.h"
#include "private/primpl.h"
#include "prlog.h"
#include "prmem.h"
#include "prmutex.h"
#include "prsemlist.h"

extern PRUint32 _PR_MD_CURRENT_THREAD(void);

/* A thread local variable used for WIN32 conditional variable implementation */
/* Must be thread local semaphore used whenever the thread is waiting for a conditional variable */
__declspec(thread) PRSemList* semWait = NULL;

#pragma warning( disable : 4100 ) /* disable unreferenced formal parameter warning */

/*
** Make the given thread wait for the given condition variable
*/
PRStatus _PR_WaitCondVar(PRUint32 threadId, PRCondVar* cvar, PRLock* externalLock, PRIntervalTime timeout)
{
    register PRStatus rv = PR_SUCCESS;
    PR_ASSERT(threadId == _PR_MD_CURRENT_THREAD());

    _PR_CVAR_LOCK(cvar);
    
    // Put the caller thread on the condition variable's wait Q
    //   Allocate the semaphore if hasn't been already.
    if(semWait == NULL)
    {
        semWait = PR_Malloc(sizeof(PRSemList)); //Allocate memory for the structure.
        memset(semWait, 0, sizeof(PRSemList));
        semWait->sem = PR_Malloc(sizeof(PRSem));
        _PR_NEW_SEM(semWait->sem, 1, 1);    //Create a new semaphore
    }
   
    PR_SEM_APPEND_LINK(semWait, &cvar->condQ);

    _PR_CVAR_UNLOCK(cvar);
 
    PR_Unlock(externalLock);

    //Wait on the semaphore...
    rv = _PR_TIMED_WAIT_SEM(semWait->sem, timeout);

    /* Reacquire lock to avoid race conditions. */
    _PR_CVAR_LOCK(cvar);

    PR_SEM_REMOVE_AND_INIT_LINK(semWait);
 
    _PR_CVAR_UNLOCK(cvar);

    /* Always regain the external lock since that's the guarantee we give to our callers. */
    PR_Lock(externalLock);
    return rv;
}


/************************************************************************/

/*
** Create a new condition variable.
** 	"lock" is the lock to use with the condition variable.
**
** Condition variables are synchronization objects that threads can use
** to wait for some condition to occur.
**
** This may fail if memory is tight or if some operating system resource
** is low.
*/

PR_IMPLEMENT(PRCondVar*) PR_NewCondVar(PRLock *lock)
{
    PRCondVar *cvar;

    PR_ASSERT(lock != NULL);

    cvar = PR_NEWZAP(PRCondVar);
    if(cvar)
	{
		cvar->lock = lock;
    	PR_SEM_INIT_CLIST(&cvar->condQ);  
       
        //printf("%d Initialized cvar condQ(%d)\n", GetCurrentThreadId(), &cvar->condQ);

		if (_PR_MD_NEW_LOCK(&(cvar->ilock)) == PR_FAILURE)
        {
            PR_DELETE(cvar);
		    return NULL;
        }
	}
	else
	{
        //PR_SetError(PR_OUT_OF_MEMORY_ERROR, 0);
    }
    return cvar;
}

/*
** Destroy a condition variable. There must be no thread
** waiting on the condvar. The caller is responsible for guaranteeing
** that the condvar is no longer in use.
**
*/
PR_IMPLEMENT(void) PR_DestroyCondVar(PRCondVar *cvar)
{
    PR_ASSERT(cvar->condQ.next == &cvar->condQ);
    _PR_MD_FREE_LOCK(&(cvar->ilock));
	PR_DELETE(cvar);
}





/*
** Wait for a notify on the condition variable. Sleep for "tiemout" amount
** of ticks (if "timeout" is zero then the sleep is indefinite). While
** the thread is waiting it unlocks lock. When the wait has
** finished the thread regains control of the condition variable after
** locking the associated lock.
**
** The thread waiting on the condvar will be resumed when the condvar is
** notified (assuming the thread is the next in line to receive the
** notify) or when the timeout elapses.
**
** Returns PR_FAILURE if the caller has not locked the lock associated
** with the condition variable or the thread has been interrupted.
*/

PR_IMPLEMENT(PRStatus) PR_WaitCondVar(PRCondVar *cvar, PRIntervalTime timeout)
{
    PRUint32 me = _PR_MD_CURRENT_THREAD();

	PR_ASSERT(cvar->lock->owner == me);
   	if (cvar->lock->owner != me)
		return PR_FAILURE;

	return _PR_WaitCondVar(me, cvar, cvar->lock, timeout);
}

/*
** Notify 1 or all threads waiting on the condition
** variable. If a thread is waiting on the condition variable (using
** PR_Wait) then it is awakened and begins waiting on the lock.
*/

PRStatus PR_UnblockCondVar(PRCondVar *cvar, PRBool bUnblockAll)
{
	PRUint32 me = _PR_MD_CURRENT_THREAD();
    PRSemList *q=NULL;
    PRSemList *next=NULL;

	PR_ASSERT(cvar->lock->owner == me);
	if (cvar->lock->owner != me)
		return PR_FAILURE;

	PR_ASSERT(cvar != NULL);
    if(cvar== NULL)
 		return PR_FAILURE;

    _PR_CVAR_LOCK(cvar);

    q = cvar->condQ.next;
    while (q != &cvar->condQ)
    {
        next = q->next;     /* Save next since we are initializing the link */
        PR_SEM_REMOVE_AND_INIT_LINK(q);
   
        _PR_POST_SEM(q->sem);   /*Release the thread...*/
        
        if(!bUnblockAll)
            break;              /*Stop after doing only 1 if not unblocking all*/
		q = next;
    }

    _PR_CVAR_UNLOCK(cvar);
    return PR_SUCCESS;
}


/*
** Notify the highest priority thread waiting on the condition
** variable. If a thread is waiting on the condition variable (using
** PR_Wait) then it is awakened and begins waiting on the lock.
*/
PR_IMPLEMENT(PRStatus) PR_NotifyCondVar(PRCondVar *cvar)
{
	return PR_UnblockCondVar(cvar, PR_FALSE);
}

/*
** Notify all of the threads waiting on the condition variable. All of
** threads are notified in turn. The highest priority thread will
** probably acquire the lock.
*/
PR_IMPLEMENT(PRStatus) PR_NotifyAllCondVar(PRCondVar *cvar)
{
	return PR_UnblockCondVar(cvar, PR_TRUE);
}

