#include "Portlet/UnavailableException.h"

Portlet_BEGIN

const int UnavailableException::PERMANENT_SECONDS = -1;


UnavailableException::UnavailableException(const std::string& msg):
PortletException(msg), _permanent(true), _seconds(PERMANENT_SECONDS)	
{ 
}


UnavailableException::UnavailableException(const std::string& msg, int seconds):
PortletException(msg), _permanent(false) 	
{
	if (seconds <= 0) _seconds = PERMANENT_SECONDS;
	else _seconds = seconds;
}


UnavailableException::~UnavailableException() throw()
{
}


Portlet_END