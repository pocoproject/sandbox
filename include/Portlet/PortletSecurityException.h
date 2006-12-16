/// PortletSecurityException.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_PortletSecurityException_INCLUDED
#define Portlet_PortletSecurityException_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletException_INCLUDED
#include "Portlet/PortletException.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletSecurityException : public PortletException
	/// The PortletSecurityException is thrown when a portlet tries to use
	/// or set a portlet mode that is not supported by the current
	/// runtime environment or the portlet.
{  
public:
	PortletSecurityException(const std::string& text);
  		/// Constructs a new portlet security exception with the given text.
  		///
  		/// Parameter text is the exception text.

private:
	PortletSecurityException();
};


Portlet_END


#endif // Portlet_PortletSecurityException_INCLUDED

