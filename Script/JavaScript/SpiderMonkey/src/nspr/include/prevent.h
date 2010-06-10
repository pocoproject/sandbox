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
** File:		prevent.h
** Description:	API to basic event functions of NSPR.
**
**
**/

#ifndef prevent_h___
#define prevent_h___

#include "prtypes.h"

PR_BEGIN_EXTERN_C


/**********************************************************************/
/************************* TYPES AND CONSTANTS ************************/
/**********************************************************************/

/*
 * PRLock --
 *
 *     NSPR represents the lock as an opaque entity to the client of the
 *	   API.  All routines operate on a pointer to this opaque entity.
 */
typedef struct PREvent PREvent;
typedef struct PRMutex PRMutex;


/**********************************************************************/
/****************************** FUNCTIONS *****************************/
/**********************************************************************/

/***********************************************************************
** FUNCTION:    PR_NewEvent
** DESCRIPTION:
**  Returns a pointer to a newly created opaque event object.
** INPUTS:      void
** OUTPUTS:     void
** RETURN:      PRLock*
**   If the lock can not be created because of resource constraints, NULL
**   is returned.
**  
***********************************************************************/
NSPR_API(PREvent*) PR_NewEvent(void);

/***********************************************************************
** FUNCTION:    PR_DestroyEvent
** DESCRIPTION:
**  Destroys a given opaque lock object.
** INPUTS:      PRLock *lock
**              Lock to be freed.
** OUTPUTS:     void
** RETURN:      None
***********************************************************************/
NSPR_API(void) PR_DestroyEvent(PREvent *event);


/***********************************************************************
** FUNCTION:    PR_SignalEventAndWaitSem
** DESCRIPTION:
**  Lock a lock.
** INPUTS:      PREvent *event
** INPUTS:      PREvent *event
**              
** OUTPUTS:     void
** RETURN:      None
***********************************************************************/
NSPR_API(PRStatus) PR_SignalEventAndWaitSem(PREvent* event, PRSem *sem, PRIntervalTime ticks);

NSPR_API(PRStatus) PR_WaitForEvent(PREvent *event, PRIntervalTime ticks);

PR_END_EXTERN_C

#endif