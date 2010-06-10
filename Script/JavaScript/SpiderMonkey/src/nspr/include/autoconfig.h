// ---------------------------------------------------------------------------
//  This section attempts to auto detect the operating system. It will set
//  up specific defines that are used by the rest of the code.
// ---------------------------------------------------------------------------

#ifndef __AUTOSENSE_H__
#define __AUTOSENSE_H__

/*
 * JSMiniNSPR configuration macros.
 */
#ifndef JSMININSPR_VERSION
#define JSMININSPR_VERSION 102
#endif

#if defined(_AIX)
    #define _PR_AIX
    #define _PR_UNIX
    #if defined(_AIXVERSION_430)
        #define _PR_AIX43  /* for use of POSIX compliant pthread functions */
    #endif
#elif defined(_SEQUENT_)
    #define _PR_PTX
    #define _PR_UNIX
#elif defined(_HP_UX) || defined(__hpux) || defined(_HPUX_SOURCE)
    #define _PR_HPUX
    #define _PR_UNIX
#elif defined(SOLARIS) || defined(__SVR4)
    #define _PR_SOLARIS
    #define _PR_UNIX
#elif defined(_SCO_DS)
    #define _PR_OPENSERVER
    #define _PR_UNIX
#elif defined(__UNIXWARE__) || defined(__USLC__)
    #define _PR_UNIXWARE
    #define _PR_UNIX
#elif defined(__BEOS__)
    #define _PR_BEOS
    #define _PR_UNIX
#elif defined(__QNXNTO__)
    #define _PR_QNX
    #define _PR_UNIX
#elif defined(__linux__)
    #define _PR_LINUX
    #define _PR_UNIX
    #if defined(__s390__)
        #define _PR_LINUX_390
    #endif
#elif defined(__FreeBSD__)
    #define _PR_FREEBSD
    #define _PR_UNIX
#elif defined(IRIX) || defined(__sgi)
    #define _PR_IRIX
    #define _PR_UNIX
#elif defined(__MVS__)
    #define _PR_OS390
    #define _PR_UNIX
#elif defined(EXM_OS390)
    #define _PR_OS390
    #define _PR_UNIX
#elif defined(__OS400__)
    #define _PR_AS400
    #define _PR_UNIX
#elif defined(__OS2__)
    #define _PR_OS2
#elif defined(__TANDEM)
    #define _PR_TANDEM
    #define _PR_UNIX
    #define _PR_CSET
#elif defined(_WIN32) || defined(WIN32) || defined(__CYGWIN__)

	#define _PR_WIN32
    #ifndef _X86_
       #define _X86_
    #endif
   
    #ifndef XP_PC
       #define XP_PC
    #endif

	#ifndef WIN32
      #define WIN32
    #endif
	
	#ifndef WINNT
       #define WINNT
	#endif WINNT
 
	#include "md/_winnt.cfg"

#elif defined(__WINDOWS__)

    /* IBM VisualAge special handling */
    #if defined(__32BIT__)
        #define _PR_WIN32
    #else
        #define _PR_WIN16
    #endif
#elif defined(__MSDRTS__)
    #define _PR_DOS

#elif defined(macintosh) || (defined(__APPLE__) && defined(__MACH__))
    #define _PR_MACOS
#elif defined(__alpha) && defined(__osf__)
    #define _PR_TRU64
#elif defined(__NetBSD__)
    #define _PR_NETBSD
#elif defined(__INTERIX)
    #define _PR_INTERIX
    #define _PR_UNIX
#else
    #error Code requires port to host OS!
#endif


// ---------------------------------------------------------------------------
//  This section attempts to autodetect the compiler being used. It will set
//  up Xerces specific defines that can be used by the rest of the code.
// ---------------------------------------------------------------------------
#if defined(__BORLANDC__)
    #define _PR_BORLAND
#elif defined(_MSC_VER)
    #define _PR_VISUALCPP
#elif defined(_PR_SOLARIS)
    #if defined(__SUNPRO_CC) && (__SUNPRO_CC >=0x500)
        #define _PR_SUNCC5
	#elif defined(__SUNPRO_CC) && (__SUNPRO_CC <0x500)
        #define _PR_SUNCC
    #elif defined(_EDG_RUNTIME_USES_NAMESPACES)
        #define _PR_SOLARIS_KAICC
    #elif defined(__GNUG__)
		#define _PR_GCC
    #else
        #error Code requires port to current development environment
    #endif
#elif defined (__QNXNTO__)
    #define _PR_QCC
#elif defined (__GNUG__) || defined(__BEOS__) || defined(__linux__) || defined(__CYGWIN__)
    #define _PR_GCC
#elif defined(_PR_HPUX)
    #if defined(EXM_HPUX)
        #define _PR_HPUX_KAICC
    #elif (__cplusplus == 1)
        #define _PR_HPUX_CC
    #elif (__cplusplus == 199707 || __cplusplus == 199711)
        #define _PR_HPUX_aCC
    #endif
#elif defined(_PR_IRIX)
    #define _PR_MIPSPRO_CC
#elif defined(_PR_PTX)
    #define _PR_PTX_CC
#elif defined(_PR_TANDEM)
    #define _PR_TANDEMCC
#elif defined(__MVS__) && defined(__cplusplus)
    #define _PR_MVSCPP
#elif defined(EXM_OS390) && defined(__cplusplus)
    #define _PR_MVSCPP
#elif defined(__IBMC__) || defined(__IBMCPP__)
    #if defined(_PR_WIN32)
        #define _PR_IBMVAW32
    #elif defined(_PR_OS2)
        #define _PR_IBMVAOS2
        #if (__IBMC__ >= 400 || __IBMCPP__ >= 400)
            #define _PR_IBMVA4_OS2
        #endif
    #elif defined(_PR_AIX)
        #define _PR_CSET              
    #endif
#elif defined(_PR_TRU64) && defined(__DECCXX)
    #define _PR_DECCXX
#elif defined(__MWERKS__)
    #define _PR_METROWERKS
#elif defined(__OS400__)
#else
    #error Code requires port to current development environment
#endif

// ---------------------------------------------------------------------------
//  The gcc compiler 2.95... is generating an internal error for some template
//  definitions. So, if we are compiling with gcc, have a specific define that
//  we can later use in the code.
// ---------------------------------------------------------------------------
#if defined(__GNUC__)
#define _PR_GCC_VERSION (__GNUC__ * 10000 \
                         + __GNUC_MINOR__ * 100 \
                         + __GNUC_PATCHLEVEL__)
#endif

#endif

