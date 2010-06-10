/* This header was stubbed out because it was included by jslock.c in spidermonkey */
/* PRThreads are no longer used in this implementation of NSPR  */
/* We only need to stub out PR_GetCurrentThread(void) and have it return the id of the thread */

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

#ifndef prthread_h___
#define prthread_h___

#include "prtypes.h"
#include "prinrval.h"

PR_BEGIN_EXTERN_C

typedef struct PRThread PRThread;

typedef enum PRThreadType {
    PR_USER_THREAD,
    PR_SYSTEM_THREAD
} PRThreadType;

typedef enum PRThreadScope {
    PR_LOCAL_THREAD,
    PR_GLOBAL_THREAD,
    PR_GLOBAL_BOUND_THREAD
} PRThreadScope;

typedef enum PRThreadState {
    PR_JOINABLE_THREAD,
    PR_UNJOINABLE_THREAD
} PRThreadState;

typedef enum PRThreadPriority
{
    PR_PRIORITY_FIRST = 0,      /* just a placeholder */
    PR_PRIORITY_LOW = 0,        /* the lowest possible priority */
    PR_PRIORITY_NORMAL = 1,     /* most common expected priority */
    PR_PRIORITY_HIGH = 2,       /* slightly more aggressive scheduling */
    PR_PRIORITY_URGENT = 3,     /* it does little good to have more than one */
    PR_PRIORITY_LAST = 3        /* this is just a placeholder */
} PRThreadPriority;

/*
** Return the current thread id for the currently running code.
** Never returns NULL.
*/
NSPR_API(PRUint32) PR_GetCurrentThread(void);

/*
** This routine returns a new index for per-thread-private data table. 
** The index is visible to all threads within a process. This index can 
** be used with the PR_SetThreadPrivate() and PR_GetThreadPrivate() routines 
** to save and retrieve data associated with the index for a thread.
**
** Each index is associationed with a destructor function ('dtor'). The function
** may be specified as NULL when the index is created. If it is not NULL, the
** function will be called when:
**      - the thread exits and the private data for the associated index
**        is not NULL,
**      - new thread private data is set and the current private data is
**        not NULL.
**
** The index independently maintains specific values for each binding thread. 
** A thread can only get access to its own thread-specific-data.
**
** Upon a new index return the value associated with the index for all threads
** is NULL, and upon thread creation the value associated with all indices for 
** that thread is NULL. 
**
** Returns PR_FAILURE if the total number of indices will exceed the maximun 
** allowed.
*/
typedef void (PR_CALLBACK *PRThreadPrivateDTOR)(void *priv);

NSPR_API(PRStatus) PR_NewThreadPrivateIndex(
    PRUintn *newIndex, PRThreadPrivateDTOR destructor);

/*
** Define some per-thread-private data.
**     "tpdIndex" is an index into the per-thread private data table
**     "priv" is the per-thread-private data 
**
** If the per-thread private data table has a previously registered
** destructor function and a non-NULL per-thread-private data value,
** the destructor function is invoked.
**
** This can return PR_FAILURE if the index is invalid.
*/
NSPR_API(PRStatus) PR_SetThreadPrivate(PRUintn tpdIndex, void *priv);

/*
** Recover the per-thread-private data for the current thread. "tpdIndex" is
** the index into the per-thread private data table. 
**
** The returned value may be NULL which is indistinguishable from an error 
** condition.
**
** A thread can only get access to its own thread-specific-data.
*/
NSPR_API(void*) PR_GetThreadPrivate(PRUintn tpdIndex);

/*
** Make the current thread sleep until "ticks" time amount of time
** has expired. If "ticks" is PR_INTERVAL_NO_WAIT then the call is
** equivalent to calling PR_Yield. Calling PR_Sleep with an argument
** equivalent to PR_INTERVAL_NO_TIMEOUT is an error and will result
** in a PR_FAILURE error return.
*/
NSPR_API(PRStatus) PR_Sleep(PRIntervalTime ticks);

/*
** Create a new thread:
**     "type" is the type of thread to create
**     "start(arg)" will be invoked as the threads "main"
**     "priority" will be created thread's priority
**     "scope" will specify whether the thread is local or global
**     "state" will specify whether the thread is joinable or not
**     "stackSize" the size of the stack, in bytes. The value can be zero
**        and then a machine specific stack size will be chosen.
**
** This can return NULL if some kind of error occurs, such as if memory is
** tight.
**
** If you want the thread to start up waiting for the creator to do
** something, enter a lock before creating the thread and then have the
** threads start routine enter and exit the same lock. When you are ready
** for the thread to run, exit the lock.
**
** If you want to detect the completion of the created thread, the thread
** should be created joinable.  Then, use PR_JoinThread to synchrnoize the
** termination of another thread.
**
** When the start function returns the thread exits. If it is the last
** PR_USER_THREAD to exit then the process exits.
*/
NSPR_API(PRThread*) PR_CreateThread(PRThreadType type,
                     void (PR_CALLBACK *start)(void *arg),
                     void *arg,
                     PRThreadPriority priority,
                     PRThreadScope scope,
                     PRThreadState state,
                     PRUint32 stackSize);

/*
** Wait for thread termination:
**     "thread" is the target thread 
**
** This can return PR_FAILURE if no joinable thread could be found 
** corresponding to the specified target thread.
**
** The calling thread is blocked until the target thread completes.
** Several threads cannot wait for the same thread to complete; one thread
** will operate successfully and others will terminate with an error PR_FAILURE.
** The calling thread will not be blocked if the target thread has already
** terminated.
*/
NSPR_API(PRStatus) PR_JoinThread(PRThread *thread);

PR_END_EXTERN_C

#endif
