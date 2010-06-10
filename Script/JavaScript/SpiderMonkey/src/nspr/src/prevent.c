#include "private/primpl.h"


PRStatus
_PR_NEW_EVENT(PREvent *event, PRBool bManualReset, PRBool bInitialState)
{
    event->hEvent = CreateEvent(NULL, bManualReset==PR_TRUE?TRUE:FALSE, bInitialState==PR_TRUE?TRUE:FALSE, NULL);
	if(event->hEvent)
		return PR_SUCCESS;

	return PR_FAILURE;
}

void 
_PR_DESTROY_EVENT(PREvent *event)
{
    CloseHandle(event->hEvent);
    event->hEvent=NULL;
}

PR_IMPLEMENT(PRStatus) PR_WaitForEvent(PREvent *event, PRIntervalTime ticks)
{
    int rv;
	if(ticks == PR_INTERVAL_NO_TIMEOUT)
	    rv = WaitForSingleObject(event->hEvent, INFINITE);
	else
	    rv = WaitForSingleObject(event->hEvent, PR_IntervalToMilliseconds(ticks));

    if (rv == WAIT_OBJECT_0)
        return PR_SUCCESS;
    
    //long liLastError = GetLastError();
    return PR_FAILURE;
}
