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

#ifndef nspr_win32_defs_h___
#define nspr_win32_defs_h___



/* Need to force service-pack 3 extensions to be defined by
** setting _WIN32_WINNT to NT 4.0 for winsock.h, winbase.h, winnt.h.
** Need 0x0403 or later for InitializeCriticalSectionAndSpinCount API
*/ 
#ifndef  _WIN32_WINNT
    #define _WIN32_WINNT 0x0500
#elif   (_WIN32_WINNT < 0x0500)
    #undef  _WIN32_WINNT
    #define _WIN32_WINNT 0x0500
#endif /* _WIN32_WINNT */

#include <windows.h>
#include <errno.h>

/* --- Lock stuff --- */
#undef PROFILE_LOCKS

struct _MDLock {
    CRITICAL_SECTION mutex;          /* this is recursive on NT */
#ifdef PROFILE_LOCKS
    PRInt32 hitcount;
    PRInt32 misscount;
#endif
};

struct CSLock {
    CRITICAL_SECTION mutex;          /* this is recursive on NT */
};


typedef struct _MDLock _MDLock;

#ifdef _DEBUG
#ifndef DEBUG
#define DEBUG
#endif
#endif

struct _MDCVar {
    PRUint32         unused;
};

struct _MDSemaphore {
    HANDLE           sem;
};


#define _MD_NEW_LOCK(lock)            (InitializeCriticalSection(&((lock)->mutex)),PR_SUCCESS)
#define _MD_FREE_LOCK(lock)           DeleteCriticalSection(&((lock)->mutex))

#define _MD_NEW_CS(lock)            (InitializeCriticalSection(&((lock)->mutex)),PR_SUCCESS)
#define _MD_FREE_CS(lock)           DeleteCriticalSection(&((lock)->mutex))
#define _MD_LOCK_CS(lock)           EnterCriticalSection(&((lock)->mutex))
#define _MD_UNLOCK_CS(lock)         LeaveCriticalSection(&((lock)->mutex))

/* --- Threading Stuff --- */
#define _MD_CREATE_THREAD           _PR_MD_CREATE_THREAD
#define _MD_JOIN_THREAD             _PR_MD_JOIN_THREAD
#define _MD_END_THREAD              _PR_MD_END_THREAD
#define _MD_YIELD                   _PR_MD_YIELD
#define _MD_SET_PRIORITY            _PR_MD_SET_PRIORITY
#define _MD_EXIT_THREAD             _PR_MD_EXIT_THREAD
#define _MD_SUSPEND_THREAD          _PR_MD_SUSPEND_THREAD
#define _MD_RESUME_THREAD           _PR_MD_RESUME_THREAD

extern PRStatus _MD_LOCK(_MDLock *mutex);
extern PRStatus _MD_UNLOCK(_MDLock *mutex);

#ifndef PROFILE_LOCKS
#define _MD_LOCK(lock)                EnterCriticalSection(&((lock)->mutex))
#define _MD_TEST_AND_LOCK(lock)       (TryEnterCriticalSection(&((lock)->mutex))== FALSE)
#define _MD_UNLOCK(lock)              LeaveCriticalSection(&((lock)->mutex))
#else
#define _MD_LOCK(lock)                 \
    PR_BEGIN_MACRO \
    BOOL rv = TryEnterCriticalSection(&((lock)->mutex)); \
    if (rv == TRUE) { \
        InterlockedIncrement(&((lock)->hitcount)); \
    } else { \
        InterlockedIncrement(&((lock)->misscount)); \
        EnterCriticalSection(&((lock)->mutex)); \
    } \
    PR_END_MACRO
#define _MD_UNLOCK(lock)              LeaveCriticalSection(&((lock)->mutex))
#endif
#define _PR_LOCK                      _MD_LOCK
#define _PR_UNLOCK					  _MD_UNLOCK

#define _PR_HAVE_ATOMIC_OPS
#define _PR_HAVE_ATOMIC_CAS

#define _MD_OPEN                      _PR_MD_OPEN
#define _MD_OPEN_FILE                 _PR_MD_OPEN_FILE
#define _MD_READ                      _PR_MD_READ
#define _MD_WRITE                     _PR_MD_WRITE
#define _MD_WRITEV                    _PR_MD_WRITEV
#define _MD_LSEEK                     _PR_MD_LSEEK
#define _MD_LSEEK64                   _PR_MD_LSEEK64
#define _MD_CLOSE_FILE(f)             _PR_MD_CLOSE(f, PR_FALSE)
#define _MD_GETFILEINFO               _PR_MD_GETFILEINFO
#define _MD_GETFILEINFO64             _PR_MD_GETFILEINFO64
#define _MD_GETOPENFILEINFO           _PR_MD_GETOPENFILEINFO
#define _MD_GETOPENFILEINFO64         _PR_MD_GETOPENFILEINFO64
#define _MD_STAT                      _PR_MD_STAT
#define _MD_RENAME                    _PR_MD_RENAME     
#define _MD_ACCESS                    _PR_MD_ACCESS     
#define _MD_DELETE                    _PR_MD_DELETE     
#define _MD_MKDIR                     _PR_MD_MKDIR      
#define _MD_MAKE_DIR                  _PR_MD_MAKE_DIR
#define _MD_RMDIR                     _PR_MD_RMDIR      
#define _MD_LOCKFILE                  _PR_MD_LOCKFILE
#define _MD_TLOCKFILE                 _PR_MD_TLOCKFILE
#define _MD_UNLOCKFILE                _PR_MD_UNLOCKFILE

#define _MD_ATOMIC_INCREMENT(x)       InterlockedIncrement((PLONG)x)
#define _MD_ATOMIC_ADD(ptr,val)		 (InterlockedExchangeAdd((PLONG)ptr, (LONG)val) + val)
#define _MD_ATOMIC_DECREMENT(x)       InterlockedDecrement((PLONG)x)
#define _MD_ATOMIC_SET(x,y)           InterlockedExchange((PLONG)x, (LONG)y)

/* --- DIR stuff --- */
#define PR_DIRECTORY_SEPARATOR        '\\'
#define PR_DIRECTORY_SEPARATOR_STR    "\\"
#define PR_PATH_SEPARATOR		';'
#define PR_PATH_SEPARATOR_STR		";"
#define _MD_ERRNO()                   GetLastError()

/* --- lock and cv waiting --- */
#define _MD_WAIT                      _PR_MD_WAIT
#define _MD_CVAR_WAIT                 _PR_MD_CVAR_WAIT
#define _MD_WAKEUP_WAITER             _PR_MD_WAKEUP_WAITER

/* --- Intervals --- */
#define _MD_INTERVAL_INIT                 _PR_MD_INTERVAL_INIT
#define _MD_GET_INTERVAL                  _PR_MD_GET_INTERVAL
#define _MD_INTERVAL_PER_SEC              _PR_MD_INTERVAL_PER_SEC
#define _MD_INTERVAL_PER_MILLISEC()       (_PR_MD_INTERVAL_PER_SEC() / 1000)
#define _MD_INTERVAL_PER_MICROSEC()       (_PR_MD_INTERVAL_PER_SEC() / 1000000)

/* --- Native-Thread Specific Definitions ------------------------------- */
extern PRUint32 _MD_CURRENT_THREAD(void);

/*
** Per thread private data
*/
extern __declspec(thread) PRUint32 tpdLength;             /* thread's current vector length */
extern __declspec(thread) void **privateData;             /* private data vector or NULL */

/* Must declare this externally for TLS Callback notifications to work! */
extern void NTAPI pr_tls_callback(void* h, unsigned long dwReason, void* pv);

/* --- Named semaphores stuff --- */
#define _PR_HAVE_NAMED_SEMAPHORES
#define _MD_OPEN_SEM            _PR_MD_OPEN_SEM
#define _MD_WAIT_SEM            _PR_MD_WAIT_SEM
#define _MD_POST_SEM            _PR_MD_POST_SEM
#define _MD_POST_MULTI_SEM      _PR_MD_POST_MULTI_SEM
#define _MD_CLOSE_SEM           _PR_MD_CLOSE_SEM
//#define _MD_DELETE_SEMAPHORE(name)    PR_SUCCESS  /* no op */

struct _MDThread {
    HANDLE           handle;
    PRUint32         id;
    void            *sp;                /* only valid when suspended */
    void (*start)(void *);              /* used by _PR_MD_CREATE_THREAD to
                                         * pass its 'start' argument to
                                         * pr_root. */
};

#endif /* nspr_win32_defs_h___ */
