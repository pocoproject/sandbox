/// PortletException.h
/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletException_INCLUDED
#define Portlet_PortletException_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletException : public std::exception 
{

public:
	PortletException();

	PortletException(const std::string& message);
      
	std::string  getCause() const;

	virtual ~PortletException() throw();

private:
	std::string _cause;
};

///inlines
inline std::string  PortletException::getCause() const
{
	return _cause;
}

Portlet_END

#endif // Portlet_PortletException_INCLUDED
