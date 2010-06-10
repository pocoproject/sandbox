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
#include "prlog.h"
#include <ctype.h>
#include <string.h>
#include "prinit.h"
#include "prlock.h"
#include "prcvar.h"
#include "pratom.h"

PRLock *_pr_sleeplock;  /* used in PR_Sleep(), classic and pthreads */

PRBool _pr_initialized = PR_FALSE;


PR_IMPLEMENT(PRBool) PR_Initialized(void)
{
    return _pr_initialized;
}

/*
 ********************************************************************
 *
 * Module initialization
 *
 ********************************************************************
 */

static struct {
    PRLock *ml;
    PRCondVar *cv;
} mod_init;

static void _PR_InitCallOnce(void) {
    mod_init.ml = PR_NewLock();
    PR_ASSERT(NULL != mod_init.ml);
    mod_init.cv = PR_NewCondVar(mod_init.ml);
    PR_ASSERT(NULL != mod_init.cv);
}

void _PR_CleanupCallOnce()
{
    if(mod_init.ml)
        PR_DestroyLock(mod_init.ml);
    mod_init.ml = NULL;
    if(mod_init.cv)
        PR_DestroyCondVar(mod_init.cv);
    mod_init.cv = NULL;
}

PR_IMPLEMENT(PRStatus) PR_CallOnce(
    PRCallOnceType *once,
    PRCallOnceFN    func)
{
    if (!_pr_initialized) _PR_ImplicitInitialization();

    if (!once->initialized) {
	if (PR_AtomicSet(&once->inProgress, 1) == 0) {
	    once->status = (*func)();
	    PR_Lock(mod_init.ml);
	    once->initialized = 1;
	    PR_NotifyAllCondVar(mod_init.cv);
	    PR_Unlock(mod_init.ml);
	} else {
	    PR_Lock(mod_init.ml);
	    while (!once->initialized) {
		PR_WaitCondVar(mod_init.cv, PR_INTERVAL_NO_TIMEOUT);
            }
	    PR_Unlock(mod_init.ml);
	}
    }
    return once->status;
}

PR_IMPLEMENT(PRStatus) PR_CallOnceWithArg(
    PRCallOnceType      *once,
    PRCallOnceWithArgFN  func,
    void                *arg)
{
    if (!_pr_initialized) _PR_ImplicitInitialization();

    if (!once->initialized) {
	if (PR_AtomicSet(&once->inProgress, 1) == 0) {
	    once->status = (*func)(arg);
	    PR_Lock(mod_init.ml);
	    once->initialized = 1;
	    PR_NotifyAllCondVar(mod_init.cv);
	    PR_Unlock(mod_init.ml);
	} else {
	    PR_Lock(mod_init.ml);
	    while (!once->initialized) {
		PR_WaitCondVar(mod_init.cv, PR_INTERVAL_NO_TIMEOUT);
            }
	    PR_Unlock(mod_init.ml);
	}
    }
    return once->status;
}

static void _PR_InitStuff(void)
{
    if (_pr_initialized) return;
    _pr_initialized = PR_TRUE;

/*
 * XXX: call _PR_InitMem only on those platforms for which nspr implements
 *	malloc, for now.
 */
#ifdef _PR_OVERRIDE_MALLOC
    _PR_InitMem();
#endif
   
    _PR_InitCallOnce(); //Added for initialization used by Javascript Date object.

    _PR_InitTPD();
}

void _PR_ImplicitInitialization(void)
{
	_PR_InitStuff();
}

/* prinit.c */


