#include "Portlet/PortletException.h"

Portlet_BEGIN

PortletException::PortletException()
{
}

PortletException::PortletException(const std::string& message) : _cause(message) 
{ 
}

PortletException::~PortletException() throw ()
{ 
}
     
Portlet_END