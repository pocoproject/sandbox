/// ActionResponse.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_ActionResponse_INCLUDED
#define Portlet_ActionResponse_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletResponse_INCLUDED
#include "Portlet/PortletResponse.h"
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


class CPP_PORTLET_DLL_API ActionResponse : public PortletResponse
	/// The ActionResponse interface represents the portlet
	/// response to an action request.
	/// It extends the PortletResponse interface to provide specific 
	/// action response functionality to portlets.<br>
	/// The portlet container creates an ActionResponse object and 
	/// passes it as argument to the portlet's processAction method.
{
public:
  virtual void setWindowState (const WindowState& windowState) = 0;
  		/// Sets the window state of a portlet to the given window state.
  		/// 
  		/// Possible values are the standard window states and any custom 
  		/// window states supported by the portal and the portlet. 
  		/// Standard window states are:
  		/// 	
  		/// 		MINIMIZED
  		/// 		NORMAL
  		/// 		MAXIMIZED
  		/// 
  		/// Parameter windowState is the new portlet window state
  		///
  		/// Throws WindowStateException
  		///                   if the portlet cannot switch to the specified window state.
  		///                   To avoid this exception the portlet can check the allowed
  		///                   window states with Request.isWindowStateAllowed().
  		/// Throws IllegalStateException
  		///                    if the method is invoked after sendRedirect has been called.

  virtual void setPortletMode (const PortletMode& portletMode) = 0;
  		/// Sets the portlet mode of a portlet to the given portlet mode.
  		/// 
  		/// Possible values are the standard portlet modes and any custom 
  		/// portlet modes supported by the portal and the portlet. Portlets 
  		/// must declare in the deployment descriptor the portlet modes they 
  		/// support for each markup type.  
  		/// Standard portlet modes are:
  		/// 	
  		/// 		EDIT
  		/// 		HELP
  		/// 		VIEW
  		/// 
  		/// 
  		/// Note: The portlet may still be called in a different window
  		///       state in the next render call, depending on the portlet container / portal.
  		/// 
  		/// Parameter portletMode is the new portlet mode
  		///
  		/// Throws		PortletModeException
  		///                   if the portlet cannot switch to this portlet mode,
  		///                   because the portlet or portal does not support it for this markup,
  		///                   or the current user is not allowed to switch to this portlet mode.
  		///                   To avoid this exception the portlet can check the allowed
  		///                   portlet modes with Request.isPortletModeAllowed().
  		///				IllegalStateException
  		///                    if the method is invoked after sendRedirect has been called.

  virtual void sendRedirect(const std::string& location) = 0; 
  		/// Instructs the portlet container to send a redirect response 
  		/// to the client using the specified redirect location URL.  
  		/// 
  		/// This method only accepts an absolute URL (e.g. 
  		/// http://my.co/myportal/mywebap/myfolder/myresource.gif)
  		/// or a full path URI (e.g. /myportal/mywebap/myfolder/myresource.gif).
  		/// If required, 
  		/// the portlet container may encode the given URL before the 
  		/// redirection is issued to the client.
  		/// 
  		/// The sendRedirect method can not be invoked after any of the 
  		/// following methods of the ActionResponse interface has been called:
  		/// 	
  		/// 		setPortletMode
  		/// 		setWindowState
  		/// 		setRenderParameter
  		/// 		setRenderParameters
  		/// 
  		///
  		/// Parameter location is the redirect location URL
  		///
  		/// Throws IOException if an input or output exception occurs.
  		/// Throws IllegalArgumentException if a relative path URL is given
  		/// Throws IllegalStateException if the method is invoked after 
		/// any of above mentioned methods of the ActionResponse interface 
  		/// has been called.

  virtual void setRenderParameters(const std::map<std::string, std::vector<std::string> >& parameters) = 0;
  		/// Sets a parameter map for the render request.
  		/// 
  		/// All previously set render parameters are cleared.
  		/// 
  		/// These parameters will be accessible in all
  		/// sub-sequent render calls via the
  		/// PortletRequest.getParameter call until
  		/// a new request is targeted to the portlet.
  		/// 
  		/// The given parameters do not need to be encoded
  		/// prior to calling this method.
  		///
  		/// Parameter parameters is map containing parameter names for 
  		///                      the render phase as 
  		///                      keys and parameter values as map 
  		///                      values. The keys in the parameter
  		///                      map must be of type String. The values 
  		///                      in the parameter map must be of type
  		///                      String array (String[]).
  		///
  		/// Throws IllegalArgumentException 
  		///                      if parameters is null, if
  		///                      any of the key/values in the Map are null, 
  		///                      if any of the keys is not a String, or if any of 
  		///                      the values is not a String array.
  		/// Throws IllegalStateException
  		///                    if the method is invoked after sendRedirect has been called.

  virtual void setRenderParameter(const std::string& key, const std::string& value) = 0;
  		/// Sets a String parameter for the render request.
  		/// 
  		/// These parameters will be accessible in all
  		/// sub-sequent render calls via the
  		/// PortletRequest.getParameter call until
  		/// a request is targeted to the portlet.
  		/// 
  		/// This method replaces all parameters with the given key.
  		/// 
  		/// The given parameter do not need to be encoded
  		/// prior to calling this method.
  		///
  		/// Parameter key is the key of the render parameter
  		/// Parameter value is the value of the render parameter
  		///
  		/// Throws IllegalArgumentException	if key or value are null.
  		/// Throws IllegalStateException
  		/// if the method is invoked after sendRedirect has been called.

  virtual void setRenderParameter(const std::string& key, const std::vector<std::string>& values) = 0;
  		/// Sets a String array parameter for the render request.
  		/// 
  		/// These parameters will be accessible in all
  		/// sub-sequent render calls via the
  		/// PortletRequest.getParameter call until
  		/// a request is targeted to the portlet.
  		/// 
  		/// This method replaces all parameters with the given key.
  		/// 
  		/// The given parameter do not need to be encoded
  		/// prior to calling this method.
  		/// 
  		/// Parameter key is the key of the render parameter
  		/// Parameter values are values of the render parameter
  		///
  		/// Throws IllegalArgumentException if key or value are null.
  		/// Throws IllegalStateException if the method is invoked after 
		/// sendRedirect has been called.
};


Portlet_END


#endif // Portlet_ActionResponse_INCLUDED
