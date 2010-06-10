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

#ifndef primpl_h___
#define primpl_h___

#ifdef WINNT
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
#endif /* WINNT */

typedef struct PRSegment PRSegment;

#include "md/prosdep.h"
#include "prinrval.h"
#include "pripcsem.h"
#include "prevent.h"
#include "prsemlist.h"
#include "prthread.h"

#ifdef _PR_HAVE_POSIX_SEMAPHORES
#include <semaphore.h>
#elif defined(_PR_HAVE_SYSV_SEMAPHORES)
#include <sys/sem.h>
#endif

/*************************************************************************
*****  A Word about Model Dependent Function Naming Convention ***********
*************************************************************************/

/*
NSPR 2.0 must implement its function across a range of platforms 
including: MAC, Windows/16, Windows/95, Windows/NT, and several
variants of Unix. Each implementation shares common code as well 
as having platform dependent portions. This standard describes how
the model dependent portions are to be implemented.

In header file pr/include/primpl.h, each publicly declared 
platform dependent function is declared as:

NSPR_API void _PR_MD_FUNCTION( long arg1, long arg2 );
#define _PR_MD_FUNCTION _MD_FUNCTION

In header file pr/include/md/<platform>/_<platform>.h, 
each #define'd macro is redefined as one of:

#define _MD_FUNCTION <blanks>
#define _MD_FUNCTION <expanded macro>
#define _MD_FUNCTION <osFunction>
#define _MD_FUNCTION <_MD_Function>

Where:

<blanks> is no definition at all. In this case, the function is not implemented 
and is never called for this platform. 
For example: 
#define _MD_INIT_CPUS()

<expanded macro> is a C language macro expansion. 
For example: 
#define        _MD_CLEAN_THREAD(_thread) \
    PR_BEGIN_MACRO \
        PR_DestroyCondVar(_thread->md.asyncIOCVar); \
        PR_DestroyLock(_thread->md.asyncIOLock); \
    PR_END_MACRO

<osFunction> is some function implemented by the host operating system. 
For example: 
#define _MD_EXIT        exit

<_MD_function> is the name of a function implemented for this platform in 
pr/src/md/<platform>/<soruce>.c file. 
For example: 
#define        _MD_GETFILEINFO         _MD_GetFileInfo

In <source>.c, the implementation is:
PR_IMPLEMENT(PRInt32) _MD_GetFileInfo(const char *fn, PRFileInfo *info);
*/

PR_BEGIN_EXTERN_C

typedef struct _MDLock _MDLock;
typedef struct _MDCVar _MDCVar;
typedef struct _MDSemaphore _MDSemaphore;
typedef struct _MDThread _MDThread;
//typedef struct _MDDir _MDDir;

struct PRSem {
#ifdef _PR_HAVE_POSIX_SEMAPHORES
    sem_t *sem;
#elif defined(_PR_HAVE_SYSV_SEMAPHORES)
    int semid;
#elif defined(WIN32)
    HANDLE sem;
#else
    PRInt8 notused;
#endif
};

/*
 * bits defined for pthreads 'state' field 
 */
#define PT_THREAD_DETACHED  0x01    /* thread can't be joined */
#define PT_THREAD_GLOBAL    0x02    /* a global thread (unlikely) */
#define PT_THREAD_SYSTEM    0x04    /* system (not user) thread */
#define PT_THREAD_PRIMORD   0x08    /* this is the primordial thread */
#define PT_THREAD_ABORTED   0x10    /* thread has been interrupted */
#define PT_THREAD_GCABLE    0x20    /* thread is garbage collectible */
#define PT_THREAD_SUSPENDED 0x40    /* thread has been suspended */
#define PT_THREAD_FOREIGN   0x80    /* thread is not one of ours */
#define PT_THREAD_BOUND     0x100    /* a bound-global thread */

typedef struct PRCondVar PRCondVar;
extern PRStatus _PR_MD_CVAR_WAIT(PRUint32 threadId, PRCondVar* cvar, PRIntervalTime ticks);
#define _PR_MD_CVAR_WAIT _MD_CVAR_WAIT

extern PRStatus _PR_MD_CREATE_THREAD(
                        PRThread *thread, 
                        void (*start) (void *), 
                        PRThreadPriority priority,                      
                        PRThreadScope scope,
                        PRThreadState state,
                        PRUint32 stackSize);
#define    _PR_MD_CREATE_THREAD _MD_CREATE_THREAD

extern PRStatus _PR_MD_JOIN_THREAD(_MDThread *md);
#define    _PR_MD_JOIN_THREAD _MD_JOIN_THREAD

extern void _PR_MD_END_THREAD(void);
#define    _PR_MD_END_THREAD _MD_END_THREAD

extern void _PR_MD_YIELD(void);
#define    _PR_MD_YIELD _MD_YIELD

extern void _PR_MD_SET_PRIORITY(_MDThread *md, PRThreadPriority newPri);
#define    _PR_MD_SET_PRIORITY _MD_SET_PRIORITY

struct PRLock
{
    _MDLock ilock;         
    PRUint32 owner;        /* Id of the thread that owns this lock */
};


struct PREvent
{
    HANDLE hEvent; 
};

struct PRMutex
{
    HANDLE hMutex; 
    PRUint32 owner;        /* Id of the thread that owns this lock */
};


typedef struct CSLock CSLock;
typedef struct PRLock PRLock;
typedef struct PREvent PREvent;
typedef struct PRMutex PRMutex;


/*************************************************************************/

/* Named semaphores related */

extern PRStatus _PR_NEW_SEM(PRSem *sem, PRUintn value, PRUintn max);

extern void _PR_DESTROY_SEM(PRSem *sem);

extern PRStatus _PR_TIMED_WAIT_SEM(PRSem *sem, PRIntervalTime timeout);

extern PRStatus _PR_WAIT_SEM(PRSem *sem);

/*extern PRStatus _PR_SIGNAL_MUTEX_AND_WAIT_SEM(PRLock* mutex, PRSem *sem, PRIntervalTime ticks);*/

extern PRStatus _PR_POST_SEM(PRSem *sem);

extern PRStatus _PR_POST_MULTI_SEM(PRSem *sem, PRInt32 iPost);

extern void _PR_CLOSE_SEM(PRSem *sem);

extern PRSem * _PR_OPEN_SEM(const char *name, PRIntn flags, PRIntn mode, PRUintn value);

/*extern PRStatus _PR_SIGNAL_EVENT_AND_WAIT_MUTEX(PRUint32 threadId, PREvent* event, PRLock *mutex, PRIntervalTime ticks);*/

typedef void (PR_CALLBACK *_PRStartFn)(void *);

/* Condition Variable support */
extern __declspec(thread) PRSemList* semWait;   /*thread specific semaphore used to wait on the conditional variable */

struct PRCondVar
{
    PRLock* lock;               /* associated lock that protects the condition */

#if defined(_PR_PTHREADS)
    pthread_cond_t cv;          /* underlying pthreads condition */
    PRInt32 notify_pending;     /* CV has destroy pending notification */
#elif defined(_PR_BTHREADS)
    sem_id    sem;              /* the underlying lock */
    sem_id    handshakeSem;     /* the lock for 'notify'-threads waiting for confirmation */
    sem_id    signalSem;        /* the lock for threads waiting for someone to notify */
    volatile int32    nw;       /* the number waiting */
    volatile int32    ns;       /* the number signalling */
    long signalBenCount;        /* the number waiting on the underlying sem */
#else /* not pthreads or Be threads */
   PRSemList condQ;                // List of semaphores from different N threads that are waiting on the condVar

//    PRCList condQ;              /* Condition variable wait Q */
    _MDLock ilock;              /* Internal Lock to protect condQ */

    /*PRInt32 nWaitersBlocked;		// Number of threads blocked
    PRLock  lockWaiters;  			// lock that guards access to waiters blocked count/block queue
*/

    /*
    PRInt32 nWaitersBlocked;
    // Number of waiting threads.

    //Special use of new CSLock since PRLock is now Mutex based instead of CS based
    CSLock lockWaiters;
    // Serialize access to <nWaitersBlocked>.

    PRSem semQueue;
    // Semaphore used to queue up threads waiting for the condition to
    // become signaled. 

    PREvent eventWaitersDone;
    // An auto-reset event used by the broadcast/signal thread to wait
    // for all the waiting thread(s) to wake up and be released from the
    // semaphore. 

    PRBool bWasBroadcast;
    // Keeps track of whether we were broadcasting or signaling.  This
    // allows us to optimize the code if we're just signaling.
*/
#endif
};

#define _PR_CVAR_LOCK(_cvar) _PR_MD_LOCK(&(_cvar)->ilock); 
#define _PR_CVAR_UNLOCK(_cvar) _PR_MD_UNLOCK(&(_cvar)->ilock);
#define _PR_CVAR_WAIT(_threadId, _cvar, _timeout) _PR_MD_CVAR_WAIT(_threadId, _cvar, _timeout); 

extern void _PR_InitTPD(void);

/*
** A boolean type with an additional "unknown" state
*/

typedef enum {
    _PR_TRI_TRUE = 1,
    _PR_TRI_FALSE = 0,
    _PR_TRI_UNKNOWN = -1
} _PRTriStateBool;

/* Overriding malloc, free, etc. */
#if !defined(_PR_NO_PREEMPT) && defined(XP_UNIX) \
        && !defined(_PR_PTHREADS) && !defined(_PR_GLOBAL_THREADS_ONLY) \
        && !defined(PURIFY) \
        && !defined(DARWIN) \
        && !defined(NEXTSTEP) \
        && !defined(QNX) \
        && !(defined (UNIXWARE) && defined (USE_SVR4_THREADS))
#define _PR_OVERRIDE_MALLOC
#endif

/* Events releated */
extern PRStatus _PR_NEW_EVENT(PREvent *event, PRBool bManualReset, PRBool bInitialState);
extern void _PR_DESTROY_EVENT(PREvent *event);


/* Locks related */

NSPR_API(PRStatus) _PR_MD_NEW_LOCK(_MDLock *md);
#define    _PR_MD_NEW_LOCK _MD_NEW_LOCK

NSPR_API(void) _PR_MD_FREE_LOCK(_MDLock *md);
#define    _PR_MD_FREE_LOCK _MD_FREE_LOCK

NSPR_API(void) _PR_MD_LOCK(_MDLock *md);
#define    _PR_MD_LOCK _MD_LOCK

/* Return 0 on success, a nonzero value on failure. */
NSPR_API(PRIntn) _PR_MD_TEST_AND_LOCK(_MDLock *md);
#define    _PR_MD_TEST_AND_LOCK _MD_TEST_AND_LOCK

NSPR_API(void) _PR_MD_UNLOCK(_MDLock *md);
#define    _PR_MD_UNLOCK _MD_UNLOCK

NSPR_API(void) _PR_MD_IOQ_LOCK(void);
#define    _PR_MD_IOQ_LOCK _MD_IOQ_LOCK

NSPR_API(void) _PR_MD_IOQ_UNLOCK(void);
#define    _PR_MD_IOQ_UNLOCK _MD_IOQ_UNLOCK

extern PRBool _pr_initialized;
extern void _PR_ImplicitInitialization(void);

/* Threads related */
NSPR_API(PRUint32) _PR_MD_CURRENT_THREAD(void);
#define    _PR_MD_CURRENT_THREAD _MD_CURRENT_THREAD


/* Time intervals */

extern PRIntervalTime _PR_MD_GET_INTERVAL(void);
#define _PR_MD_GET_INTERVAL _MD_GET_INTERVAL

extern PRIntervalTime _PR_MD_INTERVAL_PER_SEC(void);
#define _PR_MD_INTERVAL_PER_SEC _MD_INTERVAL_PER_SEC

struct PRThread {
    PRUint32 state;                 /* thread's creation state */
    PRThreadPriority priority;      /* apparent priority, loosly defined */

    void *arg;                      /* argument to the client's entry point */
    _PRStartFn startFunc;           /* the root of the client's thread */

    PRUint32 id;
    PRUint32 flags;
 
    PRUint32 threadAllocatedOnStack;/* boolean */

    _MDThread md;
};


PR_END_EXTERN_C

#endif /* primpl_h___ */
