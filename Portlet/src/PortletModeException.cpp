/// PortletModeException.cpp

#include "Portlet/PortletModeException.h"
#include "Portlet/PortletException.h"
#include "Portlet/PortletMode.h"


Portlet_BEGIN


PortletModeException::PortletModeException(const std::string& text, const PortletMode& mode):
	  PortletException(text), _mode(mode)
{
}


PortletModeException::~PortletModeException() throw()
{
}


Portlet_END

