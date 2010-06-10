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

/*
** File:            prthread.c
** Descritpion:        Implemenation for threads 
** Exports:            prthread.h
*/

#include "prtypes.h"
#include "prthread.h"
#include "private/primpl.h"
#include "prmem.h"
#include "md/prosdep.h"

PR_IMPLEMENT(PRUint32) PR_GetCurrentThread(void)
{
    PRUint32 uiThreadId;
    uiThreadId = _MD_CURRENT_THREAD();  
    return uiThreadId;
}  /* PR_GetCurrentThread */

PR_IMPLEMENT(PRStatus) PR_Sleep(PRIntervalTime ticks)
{
    PRStatus rv = PR_SUCCESS;
    if (PR_INTERVAL_NO_WAIT == ticks)
        Sleep(0);
    else
        Sleep(PR_IntervalToMilliseconds(ticks));
    return rv;
}  /* PR_Sleep */



static unsigned __stdcall
pr_root(void *arg)
{
    PRThread *thread = (PRThread *)arg;
    thread->md.start(thread);
    return 0;
}


static PRThread* _PR_CreateThread(
    PRThreadType type, void (*start)(void *arg),
    void *arg, PRThreadPriority priority, PRThreadScope scope,
    PRThreadState state, PRUint32 stackSize)
{
    int rv;
    PRThread *thred;
    type;   /* Eliminate unreferenced formal parameter warning*/
    thred = PR_NEWZAP(PRThread);
    if (NULL == thred)
    {
        //PR_SetError(PR_OUT_OF_MEMORY_ERROR, errno);
        goto done;
    }
    else
    {
        thred->arg = arg;
        thred->startFunc = start;
        thred->priority = priority;
        if (PR_UNJOINABLE_THREAD == state)
            thred->state |= PT_THREAD_DETACHED;

        rv = _PR_MD_CREATE_THREAD(thred, start, priority, scope, state, stackSize);

        if (0 != rv)
        {
            //PRIntn oserr = rv;
            PR_Free(thred);  /* all that work ... poof! */
            //PR_SetError(PR_INSUFFICIENT_RESOURCES_ERROR, oserr);
            thred = NULL;  /* and for what? */
            goto done;
        }
    }

done:
    return thred;
}  /* _PR_CreateThread */


PR_IMPLEMENT(PRThread*) PR_CreateThread(
    PRThreadType type, void (*start)(void *arg), void *arg,
    PRThreadPriority priority, PRThreadScope scope,
    PRThreadState state, PRUint32 stackSize)
{
    return _PR_CreateThread(type, start, arg, priority, scope, state, stackSize);
} /* PR_CreateThread */


PR_IMPLEMENT(PRStatus) PR_JoinThread(PRThread * thread)
{
    return _PR_MD_JOIN_THREAD(&thread->md);
} /* PR_CreateThread */
