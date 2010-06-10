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
 * NT-specific semaphore handling code.
 *
 */


#include "private/primpl.h"


PRStatus 
_PR_NEW_SEM(PRSem *sem, PRUintn value, PRUintn max)
{
    sem->sem = CreateSemaphore(NULL, value, max, NULL);
	if(sem)
		return PR_SUCCESS;

	return PR_FAILURE;
}

void 
_PR_DESTROY_SEM(PRSem *sem)
{
    CloseHandle(sem->sem);
    sem->sem = NULL;
}

PRStatus 
_PR_TIMED_WAIT_SEM(PRSem *sem, PRIntervalTime ticks)
{
    int rv;

	if(ticks == PR_INTERVAL_NO_TIMEOUT)
	    rv = WaitForSingleObject(sem->sem, INFINITE);
	else
	    rv = WaitForSingleObject(sem->sem, PR_IntervalToMilliseconds(ticks));

    if (rv == WAIT_OBJECT_0)
        return PR_SUCCESS;
    return PR_FAILURE;
}

PRStatus 
_PR_WAIT_SEM(PRSem *sem)
{
    return _PR_TIMED_WAIT_SEM(sem, PR_INTERVAL_NO_TIMEOUT);
}

PRStatus 
_PR_POST_SEM(PRSem *sem)
{
    long old_count;
    if(ReleaseSemaphore(sem->sem, 1, &old_count))
		return PR_SUCCESS;

	return PR_FAILURE;
}

PRStatus 
_PR_POST_MULTI_SEM(PRSem *sem, PRInt32 iCount)
{
    long old_count=0;

    if(ReleaseSemaphore(sem->sem, iCount, &old_count))
		return PR_SUCCESS;

	return PR_FAILURE;
}
