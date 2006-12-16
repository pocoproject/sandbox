/// WindowStateException.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_WindowStateException_INCLUDED
#define Portlet_WindowStateException_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletException_INCLUDED
#include "Portlet/PortletException.h"
#endif
#ifndef Portlet_WindowState_INCLUDED
#include "Portlet/WindowState.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API WindowStateException : public PortletException
	/// The WindowStateException is thrown when a portlet tries to use
	/// or set a portlet mode that is not supported by the current
	/// runtime environment or the portlet.
{  
public:
	WindowStateException(const std::string& text, const WindowState& state);
  		/// Constructs a new portlet mode exception when the portlet needs to throw an
  		/// exception. The exception message is based on the localized message
  		/// of the underlying exception and the portlet mode that caused this exception.
  		///
  		/// Parameter cause is the root cause.
  		/// Parameter mode is the mode causing the exception.

	~WindowStateException() throw();

	const WindowState& getState();
  		/// Returns the unsupported portlet mode causing this exception.

private:
	WindowStateException();
	WindowState _state;
};


/// inlines

inline const WindowState& WindowStateException::getState()
{
  return _state;
}


Portlet_END


#endif // Portlet_WindowStateException_INCLUDED
