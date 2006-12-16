/// PortletURL.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletURL_INCLUDED
#define Portlet_PortletURL_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_WindowState_INCLUDED
#include "Portlet/WindowState.h"
#endif
#ifndef Portlet_PortletMode_INCLUDED
#include "Portlet/PortletMode.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif
#ifndef STD_VECTOR_INCLUDED
#include <vector>
# define STD_VECTOR_INCLUDED
#endif
#ifndef STD_MAP_INCLUDED
#include <map>
# define STD_MAP_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletURL
	/// The PortletURL interface represents a URL
	/// that reference the portlet itself.
	/// 
	/// A PortletURL is created through the RenderResponse.
	/// Parameters, a portlet mode, a window state and a security level
	/// can be added to PortletURL objects. The PortletURL
	/// must be converted to a String in order to embed it into
	/// the markup generated by the portlet.
	/// 
	/// There are two types of PortletURLs:
	/// 
	/// 	Action URLs, they are created with RenderResponse.createActionURL, and 
	///     trigger an action request followed by a render request.
	/// 
	/// 	Render URLs, they are created with RenderResponse.createRenderURL, and
	///     trigger a render request.
	/// 
	/// 
	/// The string reprensentation of a PortletURL does not need to be a valid 
	/// URL at the time the portlet is generating its content. It may contain  
	/// special tokens that will be converted to a valid URL, by the portal, 
	/// before the content is returned to the client.
{
public:
	virtual void setWindowState (const WindowState& windowState) = 0;
  		/// Indicates the window state the portlet should be in, if this 
  		/// portlet URL triggers a request.
  		/// 
  		/// A URL can not have more than one window state attached to it.
  		/// If more than one window state is set only the last one set
  		/// is attached to the URL.
  		/// 
  		/// Parameter windowState is the portlet window state.
  		///
  		/// Throws WindowStateException if the portlet cannot switch to this state,
  		/// because the portal does not support this state, the portlet has not 
  		/// declared in its deployment descriptor that it supports this state, 
  		/// or the current user is not allowed to switch to this state.
  		/// The PortletRequest.isWindowStateAllowed() method can be used
  		/// to check if the portlet can set a given window state.
 
	virtual void setPortletMode(const PortletMode& portletMode) = 0;
  		/// Indicates the portlet mode the portlet must be in, if this
  		/// portlet URL triggers a request.
  		/// 
  		/// A URL can not have more than one portlet mode attached to it.
  		/// If more than one portlet mode is set only the last one set
  		/// is attached to the URL.
  		/// 
  		/// Parameter portletMode is the portlet mode
  		/// 
  		/// Throws PortletModeException if the portlet cannot switch to this mode,
  		/// because the portal does not support this mode, the portlet has not
  		/// declared in its deployment descriptor that it supports this mode for 
  		/// the current markup, or the current user is not allowed to switch to
  		/// this mode. The PortletRequest.isPortletModeAllowed() method can be used
  		/// to check if the portlet can set a given portlet mode.

	virtual void setParameter (const std::string& name, const std::string& value) = 0;
  		/// Sets the given String parameter to this URL. 
  		/// 
  		/// This method replaces all parameters with the given key.
  		/// 
  		/// The PortletURL implementation 'x-www-form-urlencoded' encodes
  		/// all  parameter names and values. Developers should not encode them.
  		/// 
  		/// A portlet container may prefix the attribute names internally 
  		/// in order to preserve a unique namespace for the portlet.
  		///
  		/// Parameter name is the parameter name
  		/// Parameter value is the parameter value
  		///
  		/// Throws IllegalArgumentException if name or value are empty.

	virtual void setParameter (const std::string& name, const std::vector<std::string>& values) = 0;
  		/// Sets the given String array parameter to this URL. 
  		/// 
  		/// This method replaces all parameters with the given key.
  		/// 
  		/// The PortletURL implementation 'x-www-form-urlencoded' encodes
  		/// all  parameter names and values. Developers should not encode them.
  		/// 
  		/// A portlet container may prefix the attribute names internally 
  		/// in order to preserve a unique namespace for the portlet.
  		///
  		/// Parameter name is the parameter name
  		/// Parameter values are the parameter values
  		///
  		/// Throws IllegalArgumentException if name or values are empty.

	virtual void setParameters(std::map<std::string, std::vector<std::string> >& parameters) = 0;
  		/// Sets a parameter map for this URL.
  		/// 
  		/// All previously set parameters are cleared.
  		/// 
  		/// The PortletURL implementation 'x-www-form-urlencoded' encodes
  		/// all  parameter names and values. Developers should not encode them.
  		/// 
  		/// A portlet container may prefix the attribute names internally, 
  		/// in order to preserve a unique namespace for the portlet.
  		///
  		/// Parameter parameters is a map containing parameter names for 
  		/// the render phase as keys and parameter values as map 
  		/// values. 
  		///
  		/// Throws IllegalArgumentException if parameters is empty, if
  		/// any of the key/values in the map are empty.

	virtual void setSecure (bool secure) = 0;
  		/// Indicated the security setting for this URL. 
  		/// 
  		/// Secure set to true indicates that the portlet requests
  		/// a secure connection between the client and the portlet window for
  		/// this URL. Secure set to false indicates that the portlet 
  		/// does not need a secure connection for this URL. If the security is not
  		/// set for a URL, it will stay the same as the current request. 
  		///
  		/// Parameter secure true, if portlet requests to have a secure connection
  		/// between its portlet window and the client; false, if the portlet
  		/// does not require a secure connection.
  		///
  		/// Throws PortletSecurityException if the run-time environment does
  		/// not support the indicated setting.

	virtual const std::string& toString() const = 0;
  		/// Returns the portlet URL string representation to be embedded in the
  		/// markup.
  		/// Note that the returned String may not be a valid URL, as it may
  		/// be rewritten by the portal/portlet-container before returning the 
  		/// markup to the client.
};


Portlet_END


#endif //Portlet_PortletURL_INCLUDED