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

#include "prtypes.h"
#include "prlock.h"
#include "private/primpl.h"
#include "prmem.h"
#include "prlog.h"

/************************************************************************/


PR_IMPLEMENT(PRLock*) PR_NewLock(void)
{
    PRLock* lock;

    lock = PR_NEWZAP(PRLock);
    if (lock) {
        if (_PR_MD_NEW_LOCK(&lock->ilock) == PR_FAILURE) {
			PR_DELETE(lock);
			return(NULL);
		}
    }
    return lock;
}

/*
** Destroy the given lock "lock". There is no point in making this race
** free because if some other thread has the pointer to this lock all
** bets are off.
*/
PR_IMPLEMENT(void) PR_DestroyLock(PRLock *lock)
{
    PR_ASSERT(lock->owner == 0);
	_PR_MD_FREE_LOCK(&lock->ilock);
    PR_DELETE(lock);
}

/*
** Lock the lock.
*/
PR_IMPLEMENT(void) PR_Lock(PRLock *lock)
{
    PRUint32 me;

    PR_ASSERT(lock != NULL);
	me = _PR_MD_CURRENT_THREAD();
	PR_ASSERT(lock->owner != me);
	_PR_MD_LOCK(&lock->ilock);
    lock->owner = me;
}

/*
** Unlock the lock.
*/
PR_IMPLEMENT(PRStatus) PR_Unlock(PRLock *lock)
{
    lock->owner = 0;
	_PR_MD_UNLOCK(&lock->ilock);
	return PR_SUCCESS;
}


