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

#include "private/primpl.h"
#include <process.h>  /* for _beginthreadex() */
#include "prlog.h"

void     
_PR_MD_SET_PRIORITY(_MDThread *thread, PRThreadPriority newPri)
{
    int nativePri;
    BOOL rv;

    if (newPri < PR_PRIORITY_FIRST) {
        newPri = PR_PRIORITY_FIRST;
    } else if (newPri > PR_PRIORITY_LAST) {
        newPri = PR_PRIORITY_LAST;
    }
    switch (newPri) {
        case PR_PRIORITY_LOW:
            nativePri = THREAD_PRIORITY_BELOW_NORMAL;
            break;
        case PR_PRIORITY_NORMAL:
            nativePri = THREAD_PRIORITY_NORMAL;
            break;
        case PR_PRIORITY_HIGH:
            nativePri = THREAD_PRIORITY_ABOVE_NORMAL;
            break;
        case PR_PRIORITY_URGENT:
            nativePri = THREAD_PRIORITY_HIGHEST;
            break;
        default:
            nativePri = THREAD_PRIORITY_NORMAL;
            break;
    }
    rv = SetThreadPriority(thread->handle, nativePri);
    PR_ASSERT(rv);
    /*if (!rv) {
	PR_LOG(_pr_thread_lm, PR_LOG_MIN,
                ("PR_SetThreadPriority: can't set thread priority\n"));
}
    */
    return;
}

static unsigned __stdcall
pr_root(void *arg)
{
    PRThread *thread = (PRThread *)arg;
    thread->md.start(thread->arg);
    return 0;
}

PRStatus 
_PR_MD_CREATE_THREAD(PRThread *thread, 
                  void (*start)(void *), 
                  PRThreadPriority priority, 
                  PRThreadScope scope, 
                  PRThreadState state, 
                  PRUint32 stackSize)
{
    scope;
    state;
    thread->md.start = start;
    thread->md.handle = (HANDLE) _beginthreadex(
                    NULL,
                    stackSize,
                    pr_root,
                    (void *)thread,
                    CREATE_SUSPENDED,
                    &(thread->id));
    if(!thread->md.handle) {
        /*
        PRErrorCode prerror;
        thread->errorCode = GetLastError();
        switch (errno) {
            case ENOMEM:
                prerror = PR_OUT_OF_MEMORY_ERROR;
                break;
            case EAGAIN:
                prerror = PR_INSUFFICIENT_RESOURCES_ERROR;
                break;
            case EINVAL:
                prerror = PR_INVALID_ARGUMENT_ERROR;
                break;
            default:
                prerror = PR_UNKNOWN_ERROR;
        }
        PR_SetError(prerror, errno);
        */
        return PR_FAILURE;
    }

    thread->md.id = thread->id;
    /*
     * On windows, a thread is created with a thread priority of
     * THREAD_PRIORITY_NORMAL.
     */
    if (priority != PR_PRIORITY_NORMAL) {
        _PR_MD_SET_PRIORITY(&(thread->md), priority);
    }

    /* Activate the thread */
    if ( ResumeThread( thread->md.handle ) != -1)
        return PR_SUCCESS;

    //PR_SetError(PR_UNKNOWN_ERROR, GetLastError());
    return PR_FAILURE;
}

PRStatus
_PR_MD_JOIN_THREAD(_MDThread *md)
{
    DWORD rv;

    rv = WaitForSingleObject(md->handle, INFINITE);
    PR_ASSERT(WAIT_OBJECT_0 == rv);
    if(WAIT_OBJECT_0 == rv)
        return PR_SUCCESS;
    return PR_FAILURE;
}

void
_PR_MD_END_THREAD(void)
{
    _endthreadex(0);
}

void    
_PR_MD_YIELD(void)
{
    /* Can NT really yield at all? */
    Sleep(0);
}

/*
PRInt32 _PR_MD_SETTHREADAFFINITYMASK(PRThread *thread, PRUint32 mask )
{
    int rv;

    rv = SetThreadAffinityMask(thread->md.handle, mask);

    return rv?0:-1;
}

PRInt32 _PR_MD_GETTHREADAFFINITYMASK(PRThread* thread, PRUint32 *mask)
{
    PRInt32 rv, system_mask;
    thread;

    rv = GetProcessAffinityMask(GetCurrentProcess(), mask, &system_mask);
    
    return rv?0:-1;
}
*/

void
_PR_MD_SUSPEND_THREAD(PRThread *thread)
{
    /*
    ** There seems to be some doubt about whether or not SuspendThread
    ** is a synchronous function. The test afterwards is to help veriry
    ** that it is, which is what Microsoft says it is.
    */
    PRUintn rv = SuspendThread(thread->md.handle);
    PR_ASSERT(0xffffffffUL != rv);
}

void
_PR_MD_RESUME_THREAD(PRThread *thread)
{
    ResumeThread(thread->md.handle);
}
