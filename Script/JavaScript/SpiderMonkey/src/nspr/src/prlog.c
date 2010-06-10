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
 * Contributors:
 *
 * This Original Code has been modified by IBM Corporation.
 * Modifications made by IBM described herein are
 * Copyright (c) International Business Machines Corporation, 2000.
 * Modifications to Mozilla code or documentation identified per
 * MPL Section 3.3
 *
 * Date         Modified by     Description of modification
 * 04/10/2000   IBM Corp.       Added DebugBreak() definitions for OS/2
 */

#include "private/primpl.h"
#include <string.h>

#if defined(XP_PC)
#define strcasecmp stricmp
#define strncasecmp strnicmp
#endif

/*
 * On NT, we can't define _PUT_LOG as PR_Write or _PR_MD_WRITE,
 * because every asynchronous file io operation leads to a fiber context
 * switch.  So we define _PUT_LOG as fputs (from stdio.h).  A side
 * benefit is that fputs handles the LF->CRLF translation.  This
 * code can also be used on other platforms with file stream io.
 */
#if defined(WIN32) || defined(XP_OS2)
#define _PR_USE_STDIO_FOR_LOGGING
#pragma warning( disable : 4100 ) /* disable unreferenced formal parameter warning */
#endif

/*
** Coerce Win32 log output to use OutputDebugString() when
** NSPR_LOG_FILE is set to "WinDebug".
*/
#if defined(XP_PC)
#define WIN32_DEBUG_FILE (FILE*)-2
#endif


PR_IMPLEMENT(void) PR_Abort(void)
{
    abort();
}

#if defined(XP_OS2)
/*
 * Added definitions for DebugBreak() for 2 different OS/2 compilers.
 * Doing the int3 on purpose for Visual Age so that a developer can
 * step over the instruction if so desired.  Not always possible if
 * trapping due to exception handling IBM-AKR
 */
#if defined(XP_OS2_VACPP)
#include <builtin.h>
static void DebugBreak(void) { _interrupt(3); }
#elif defined(XP_OS2_EMX)
static void DebugBreak(void) { asm("int $3"); }
#else
static void DebugBreak(void) { }
#endif
#endif /* XP_OS2 */

PR_IMPLEMENT(void) PR_Assert(const char *s, const char *file, PRIntn ln)
{
#if defined(XP_UNIX) || defined(XP_OS2) || defined(XP_BEOS)
    fprintf(stderr, "Assertion failure: %s, at %s:%d\n", s, file, ln);
#endif
#ifdef XP_MAC
    dprintf("Assertion failure: %s, at %s:%d\n", s, file, ln);
#endif
#if defined(WIN32) || defined(XP_OS2)
    DebugBreak();
#endif
#ifndef XP_MAC
    abort();
#endif
}

#ifdef XP_MAC
PR_IMPLEMENT(void) PR_Init_Log(void)
{
	_PR_InitLog();
}
#endif
