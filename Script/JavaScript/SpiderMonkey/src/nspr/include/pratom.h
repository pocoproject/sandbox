#ifndef pratom_h___
#define pratom_h___

#include "prtypes.h"
#include "prlock.h"

PR_BEGIN_EXTERN_C

/*
** FUNCTION: PR_AtomicIncrement
** DESCRIPTION:
**    Atomically increment a 32 bit value.
** INPUTS:
**    val:  a pointer to the value to increment
** RETURN:
**    the returned value is the result of the increment
*/
NSPR_API(PRInt32)	PR_AtomicIncrement(PRInt32 *val);

/*
** FUNCTION: PR_AtomicDecrement
** DESCRIPTION:
**    Atomically decrement a 32 bit value.
** INPUTS:
**    val:  a pointer to the value to decrement
** RETURN:
**    the returned value is the result of the decrement
*/
NSPR_API(PRInt32)	PR_AtomicDecrement(PRInt32 *val);

/*
** FUNCTION: PR_AtomicSet
** DESCRIPTION:
**    Atomically set a 32 bit value.
** INPUTS:
**    val: A pointer to a 32 bit value to be set
**    newval: The newvalue to assign to val
** RETURN:
**    Returns the prior value
*/
NSPR_API(PRInt32) PR_AtomicSet(PRInt32 *val, PRInt32 newval);

/*
** FUNCTION: PR_AtomicAdd
** DESCRIPTION:
**    Atomically add a 32 bit value.
** INPUTS:
**    ptr:  a pointer to the value to increment
**	  val:  value to be added
** RETURN:
**    the returned value is the result of the addition
*/
NSPR_API(PRInt32)	PR_AtomicAdd(PRInt32 *ptr, PRInt32 val);

PR_END_EXTERN_C

#endif /* pratom_h___ */
