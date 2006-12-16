/// PortletSecurityException.cpp


#include "Portlet/PortletSecurityException.h"
#include "Portlet/PortletException.h"

Portlet_BEGIN


PortletSecurityException::PortletSecurityException(const std::string& text) : 
	PortletException(text)
{
}


Portlet_END
