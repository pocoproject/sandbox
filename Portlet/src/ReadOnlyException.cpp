/// ReadOnlyException.cpp


#include "Portlet/ReadOnlyException.h"


Portlet_BEGIN


ReadOnlyException::ReadOnlyException(const std::string text) :
	PortletException(text)
{
}


ReadOnlyException::~ReadOnlyException() throw()
{
}


Portlet_END
