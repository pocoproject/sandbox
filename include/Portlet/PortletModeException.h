/// PortletModeException.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_PortletModeException_INCLUDED
#define Portlet_PortletModeException_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletMode_INCLUDED
#include "Portlet/PortletMode.h"
#endif
#ifndef Portlet_PortletException_INCLUDED
#include "Portlet/PortletException.h"
#endif

Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletModeException : public PortletException
	/// The PortletModeException is thrown when a portlet tries to use
	/// or set a portlet mode that is not supported by the current
	/// runtime environment or the portlet.
{  
public:
	PortletModeException(const std::string& text, const PortletMode& mode);
  		/// Constructs a new portlet mode exception when the portlet needs to throw an
  		/// exception. The exception message is based on the localized message
  		/// of the underlying exception and the portlet mode that caused this exception.
  		///
  		/// Parameter cause is the root cause.
  		/// Parameter mode is the mode causing the exception.
	
	~PortletModeException() throw();

	const PortletMode& getMode();
  		/// Returns the unsupported portlet mode causing this exception.

private:
	PortletModeException();
	PortletMode _mode;
};


/// inlines

inline const PortletMode& PortletModeException::getMode()
{
  return _mode;
}


Portlet_END


#endif // Portlet_PortletModeException_INCLUDED
