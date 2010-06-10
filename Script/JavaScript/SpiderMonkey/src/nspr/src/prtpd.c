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
** Thread Private Data
**
** There is an aribitrary limit on the number of keys that will be allocated
** by the runtime. It's largish, so it is intended to be a sanity check, not
** an impediment.
**
** There is a counter, initialized to zero and incremented every time a
** client asks for a new key, that holds the high water mark for keys. All
** threads logically have the same high water mark and are permitted to
** ask for TPD up to that key value.
**
** The vector to hold the TPD are allocated when PR_SetThreadPrivate() is
** called. The size of the vector will be some value greater than or equal
** to the current high water mark. Each thread has its own TPD length and
** vector.
**
** Threads that get private data for keys they have not set (or perhaps
** don't even exist for that thread) get a NULL return. If the key is
** beyond the high water mark, an error will be returned.
*/

/*
** As of this time, BeOS has its own TPD implementation.  Integrating
** this standard one is a TODO for anyone with a bit of spare time on
** their hand.  For now, we just #ifdef out this whole file and use
** the routines in pr/src/btthreads/
*/

#ifndef XP_BEOS

#include "private/primpl.h"
#include "prthread.h"
#include "prcvar.h"
#include "prmem.h"
#include "prlog.h"
#include "pratom.h"

#include <string.h>

extern void _PR_CleanupCallOnce();

#if defined(WIN95)
/*
** Some local variables report warnings on Win95 because the code paths 
** using them are conditioned on HAVE_CUSTOME_USER_THREADS.
** The pragma suppresses the warning.
** 
*/
#pragma warning(disable : 4101)
#endif

#pragma warning( disable : 4100 ) /* disable unreferenced formal parameter warning */

#define _PR_TPD_LIMIT 128               /* arbitary limit on the TPD slots */

static PRInt32 _pr_tpd_length = 0;      /* current length of destructor vector */
static PRInt32 _pr_tpd_highwater = 0;   /* next TPD key to be assigned */

/* the destructors are associated with the keys, therefore asserting that the TPD key depicts the data's 'type' */
static PRThreadPrivateDTOR *_pr_tpd_destructors = NULL;

/*
** Per thread private data
In the VC++ world (6, 7 or 8) __declspec(thread) will not work properly for these variables.
We MUST use the TLSAlloc and TLSFree stuff to guarantee TLS callbacks are wired in properly
as well as to alloc LoadLibrary calls on any DLL that uses us without causing an exception and death
*/
PRUint32 iLengthTLSKey=TLS_OUT_OF_INDEXES;            /* Index for the tpdLength variable we store in TLS */
PRUint32 iPrivateDataTLSKey=TLS_OUT_OF_INDEXES;       /* Index for the privateData variable we store in TLS */

/*
** Initialize the thread private data manipulation
*/
void _PR_InitTPD(void)
{
    //Allocate to TLS indexes for the two variables above that we are keeping in thread local storage.
    iLengthTLSKey = TlsAlloc();
    iPrivateDataTLSKey = TlsAlloc();
    //For linux we could use code similar to below.
    //int res = pthread_key_create(&temp->native_key, &cleanup_slots);

    _pr_tpd_destructors = (PRThreadPrivateDTOR*)
        PR_CALLOC(_PR_TPD_LIMIT * sizeof(PRThreadPrivateDTOR*));
    PR_ASSERT(NULL != _pr_tpd_destructors);
    _pr_tpd_length = _PR_TPD_LIMIT;
}

/*
** Clean up the thread private data manipulation
*/
void _PR_CleanupTPD(void)
{
    if(_pr_tpd_destructors != NULL)
    {    
        PR_Free(_pr_tpd_destructors);
        _pr_tpd_destructors = NULL;
    }

    //Give back our thread local storage indexes we created in _PR_InitTPD
    if(TLS_OUT_OF_INDEXES != iLengthTLSKey)
        TlsFree(iLengthTLSKey);
    if(TLS_OUT_OF_INDEXES != iPrivateDataTLSKey)
        TlsFree(iPrivateDataTLSKey);
    iLengthTLSKey=TLS_OUT_OF_INDEXES;
    iPrivateDataTLSKey=TLS_OUT_OF_INDEXES;
}  /* _PR_CleanupTPD */

/*
** This routine returns a new index for per-thread-private data table. 
** The index is visible to all threads within a process. This index can 
** be used with the PR_SetThreadPrivate() and PR_GetThreadPrivate() routines 
** to save and retrieve data associated with the index for a thread.
**
** The index independently maintains specific values for each binding thread. 
** A thread can only get access to its own thread-specific-data.
**
** Upon a new index return the value associated with the index for all threads
** is NULL, and upon thread creation the value associated with all indices for 
** that thread is NULL. 
**
**     "dtor" is the destructor function to invoke when the private
**       data is set or destroyed
**
** Returns PR_FAILURE if the total number of indices will exceed the maximun 
** allowed.
*/

PR_IMPLEMENT(PRStatus) PR_NewThreadPrivateIndex(
    PRUintn *newIndex, PRThreadPrivateDTOR dtor)
{
    PRStatus rv;
    PRInt32 index;

    if (!_pr_initialized) _PR_ImplicitInitialization();

    PR_ASSERT(NULL != newIndex);
    PR_ASSERT(NULL != _pr_tpd_destructors);

    index = PR_AtomicIncrement(&_pr_tpd_highwater) - 1;  /* allocate index */
    if (_PR_TPD_LIMIT <= index)
    {
        /*PR_SetError(PR_TPD_RANGE_ERROR, 0);  commented out since JS doesn't call PR_GetError anyway */
        rv = PR_FAILURE;  /* that's just wrong */
    }
    else
    {
        _pr_tpd_destructors[index] = dtor;  /* record destructor @index */
        *newIndex = (PRUintn)index;  /* copy into client's location */
        rv = PR_SUCCESS;  /* that's okay */
    }

    return rv;
}

/*
** Define some per-thread-private data.
**     "index" is an index into the per-thread private data table
**     "priv" is the per-thread-private data 
**
** If the per-thread private data table has a previously registered
** destructor function and a non-NULL per-thread-private data value,
** the destructor function is invoked.
**
** This can return PR_FAILURE if index is invalid (ie., beyond the current
** high water mark) or memory is insufficient to allocate an exanded vector.
*/

PR_IMPLEMENT(PRStatus) PR_SetThreadPrivate(PRUintn index, void *priv)
{
    PRUint32 tpdLength=0;
    void** privateData =NULL;

    /*
    ** The index being set might not have a sufficient vector in this
    ** thread. But if the index has been allocated, it's okay to go
    ** ahead and extend this one now.
    */
    if ((index >= _PR_TPD_LIMIT) || (index >= (PRUintn)_pr_tpd_highwater))
    {
        /* PR_SetError(PR_TPD_RANGE_ERROR, 0);  commented out since JS doesn't call PR_GetError anyway */
        return PR_FAILURE;
    }
    tpdLength = (PRUint32)TlsGetValue(iLengthTLSKey);
    privateData  = TlsGetValue(iPrivateDataTLSKey);

    PR_ASSERT(((NULL == privateData) && (0 == tpdLength))
        || ((NULL != privateData) && (0 != tpdLength)));

    if ((NULL == privateData) || (tpdLength <= index))
    {
        void *extension = PR_CALLOC(_pr_tpd_length * sizeof(void*));
        if (NULL == extension)
        {
            /*PR_SetError(PR_OUT_OF_MEMORY_ERROR, 0);  commented out since JS doesn't call PR_GetError anyway */
            return PR_FAILURE;
        }

        //Looks like we are extending the private data here.
        if (privateData)
        {
            (void)memcpy(extension, privateData, tpdLength * sizeof(void*));
            PR_DELETE(privateData);
        }

        tpdLength = _pr_tpd_length;
        privateData = (void**)extension;
        //Save the new values in thread local storage.
        TlsSetValue(iLengthTLSKey, (void*)tpdLength);
        TlsSetValue(iPrivateDataTLSKey, privateData);
    }
    /*
    ** There wasn't much chance of having to call the destructor
    ** unless the slot already existed.
    */
    else if (privateData[index] && _pr_tpd_destructors[index])
    {
        void *data = privateData[index];
        privateData[index] = NULL;
        (*_pr_tpd_destructors[index])(data);
    }

    PR_ASSERT(index < tpdLength);
    privateData[index] = priv;

    return PR_SUCCESS;
}

/*
** Recover the per-thread-private data for the current thread. "index" is
** the index into the per-thread private data table. 
**
** The returned value may be NULL which is indistinguishable from an error 
** condition.
**
*/

PR_IMPLEMENT(void*) PR_GetThreadPrivate(PRUintn index)
{
    
    void** privateData=0;
    void* tpd =0;
    PRUint32 tpdLength = 0;
    tpdLength = (PRUint32)TlsGetValue(iLengthTLSKey);
 
    privateData  = TlsGetValue(iPrivateDataTLSKey);
    tpd = ((NULL == privateData) ? NULL : privateData[index]);
    return tpd;
}

/*
** Destroy the thread's private data, if any exists. This is called at
** thread termination time only. There should be no threading issues
** since this is being called by the thread itself.
*/
void _PR_DestroyThreadPrivate()
{
#define _PR_TPD_DESTRUCTOR_ITERATIONS 4
    //Did we allocate any thread local storage?
    if(TLS_OUT_OF_INDEXES != iPrivateDataTLSKey)
    {
        void** privateData  = TlsGetValue(iPrivateDataTLSKey);
        PRBool clean;
        PRUint32 index;
        PRInt32 passes = _PR_TPD_DESTRUCTOR_ITERATIONS;
        PRUint32 tpdLength=0;
        if (NULL != privateData)  /* we have some */
        {
            tpdLength = (PRUint32)TlsGetValue(iLengthTLSKey);
            PR_ASSERT(0 != tpdLength);
            do
            {
                clean = PR_TRUE;
                for (index = 0; index < tpdLength; ++index)
                {
                    void *priv = privateData[index];  /* extract */
                    if (NULL != priv)  /* we have data at this index */
                    {
                        if(_pr_tpd_destructors && NULL != _pr_tpd_destructors[index])
                        {
                            privateData[index] = NULL;  /* precondition */
                            (*_pr_tpd_destructors[index])(priv);  /* destroy */
                            clean = PR_FALSE;  /* unknown side effects */
                        }
                    }
                }
            } while ((--passes > 0) && !clean);  /* limit # of passes */
            /*
            ** We give up after a fixed number of passes. Any non-NULL
            ** thread-private data value with a registered destructor
            ** function is not destroyed.
            */
            memset(privateData, 0, tpdLength * sizeof(void*));
            tpdLength = 0;
            PR_DELETE(privateData);
            privateData=NULL;

            TlsSetValue(iLengthTLSKey, (void*)tpdLength);
            TlsSetValue(iPrivateDataTLSKey, privateData);
        }
    }
    /* Also clean up and conditional variable stuff that may have been allocated by the thread. */
    if(semWait)
    {
        _PR_DESTROY_SEM(semWait->sem);
        PR_Free(semWait->sem);
        PR_Free(semWait);
        semWait=NULL;
      }

}  /* _PR_DestroyThreadPrivate */


#ifdef WIN32
    #include "tls_hooks.h"
    #include <stdlib.h>

    #define WIN32_LEAN_AND_MEAN
    #include <windows.h>

    /*Definitions required by implementation*/

    #if (_MSC_VER < 1310) // 1310 == VC++ 7.1
        typedef void (__cdecl *_PVFV)(void);
        #define INIRETSUCCESS
        #define PVAPI void
    #else
        typedef int (__cdecl *_PVFV)(void);
        #define INIRETSUCCESS 0
        #define PVAPI int
    #endif

    typedef void (NTAPI* _TLSCB)(HINSTANCE, DWORD, PVOID);

    /*Symbols for connection to the runtime environment */

    extern DWORD _tls_used; //the tls directory (located in .rdata segment)
    extern _TLSCB __xl_a[], __xl_z[];    //tls initializers */

    /*Forward declarations */

    static PVAPI on_tls_prepare(void);
    static PVAPI on_process_init(void);
    static PVAPI on_process_term(void);
    static void NTAPI on_tls_callback(HINSTANCE, DWORD, PVOID);

    /*The .CRT$Xxx information is taken from Codeguru:
      http://www.codeguru.com/Cpp/misc/misc/threadsprocesses/article.php/c6945__2/
    */

    #if (_MSC_VER >= 1310) // 1310 == VC++ 7.1
    #   pragma data_seg(push, old_seg)
    #endif
        /* 
           Callback to run tls glue code first.
           I don't think it is necessary to run it
           at .CRT$XIB level, since we are only
           interested in thread detachement. But
           this could be changed easily if required.
        */

        #pragma data_seg(".CRT$XIU")
        static _PVFV p_tls_prepare = on_tls_prepare;
        #pragma data_seg()

        /*Callback after all global ctors.*/

        #pragma data_seg(".CRT$XCU")
        static _PVFV p_process_init = on_process_init;
        #pragma data_seg()

        /*Callback for tls notifications.*/

        #pragma data_seg(".CRT$XLB")
        _TLSCB p_thread_callback = on_tls_callback;
        #pragma data_seg()

        /*Callback for termination.*/

        #pragma data_seg(".CRT$XTU")
        static _PVFV p_process_term = on_process_term;
        #pragma data_seg()

    #if (_MSC_VER >= 1310) // 1310 == VC++ 7.1
    #   pragma data_seg(pop, old_seg)
    #endif

    PVAPI on_tls_prepare(void)
    {
        /*
            The following line has an important side effect:
            if the TLS directory is not already there, it will
            be created by the linker. In other words, it forces a tls
            directory to be generated by the linker even when static tls
            (i.e. __declspec(thread)) is not used.
            The volatile should prevent the optimizer
            from removing the reference.
        */
        DWORD volatile dw = _tls_used;
        dw = 0;

        #if (_MSC_VER < 1310) // 1310 == VC++ 7.1
            _TLSCB* pfbegin = __xl_a;
            _TLSCB* pfend = __xl_z;
            _TLSCB* pfdst = pfbegin;
            /*
            //pfdst = (_TLSCB*)_tls_used.AddressOfCallBacks;

            //The following loop will merge the address pointers
            //into a contiguous area, since the tlssup code seems
            //to require this (at least on MSVC 6)
            */
            while (pfbegin < pfend)
            {
                if (*pfbegin != 0)
                {
                    *pfdst = *pfbegin;
                    ++pfdst;
                }
                ++pfbegin;
            }

            *pfdst = 0;
        #endif

        return INIRETSUCCESS;
    }

    PVAPI on_process_init(void)
    {
        return INIRETSUCCESS;
    }

    PVAPI on_process_term(void)
    {
        _PR_CleanupTPD();
        _PR_CleanupCallOnce();

        return INIRETSUCCESS;
    }

    void NTAPI on_tls_callback(HINSTANCE h, DWORD dwReason, PVOID pv)
    {
        switch (dwReason)
        {
            case DLL_THREAD_DETACH:
            {
                _PR_DestroyThreadPrivate();
                break;
            }
            case DLL_PROCESS_DETACH:
            {
                _PR_DestroyThreadPrivate(); //Also cleaup main threads privates (if any)
                on_process_term();  //Added 2-3-2008 because on_process_term() was not being called at process termination
                                    //which caused several 1 time leaks memory diagnostic tools.
                break;
            }
        }
    }
#endif


#endif /* !XP_BEOS */
