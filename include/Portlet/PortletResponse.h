/// PortletResponse.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletResponse_INCLUDED
#define Portlet_PortletResponse_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletResponse
	/// The PortletResponse defines the base interface to assist a
	/// portlet in creating and sending a response to the client.
	/// The portlet container uses two specialized versions of this interface
	/// when invoking a portlet, ActionResponse and
	/// RenderResponse. The  portlet container creates these
	/// objects and passes them as arguments to the portlet's processAction
	/// and render methods.
{
public:
	virtual void addProperty(const std::string& key, const std::string& value) = 0;
  		/// Adds a string property to an existing key to be returned to the portal.
  		/// 
  		/// This method allows response properties to have multiple values.
  		/// 
  		/// Properties can be used by portlets to provide vendor specific 
  		/// information to the portal.
  		///
  		/// Parameter  key is the key of the property to be returned to the portal
  		/// Parameter  value is the value of the property to be returned to the portal
  		///
  		/// Throws IllegalArgumentException if key is empty string.

	virtual void setProperty(const std::string& key, const std::string& value) = 0;
  		/// Sets a string property to be returned to the portal.
  		/// 
  		/// Properties can be used by portlets to provide vendor specific 
  		/// information to the portal.
  		/// 
  		/// This method resets all properties previously added with the same key.
  		///
  		/// Parameter  key  is  the key of the property to be returned to the portal
  		/// Parameter  value is the value of the property to be returned to the portal
  		///
  		/// Throws IllegalArgumentException if key is empty string.

	virtual const std::string& encodeURL (const std::string& path) = 0;
  		/// Returns the encoded URL of the resource, like servlets,
  		/// JSPs, images and other static files, at the given path.
  		/// 
  		/// Some portal/portlet-container implementation may require 
  		/// those URLs to contain implementation specific data encoded
  		/// in it. Because of that, portlets should use this method to 
  		/// create such URLs.
  		/// 
  		/// The encodeURL method may include the session ID 
  		/// and other portal/portlet-container specific information into the URL. 
  		/// If encoding is not needed, it returns the URL unchanged. 
  		///
  		/// Parameter   path
  		///          the URI path to the resource. This must be either
  		///          an absolute URL (e.g. 
  		///          http://my.co/myportal/mywebap/myfolder/myresource.gif)
  		///          or a full path URI (e.g. /myportal/mywebap/myfolder/myresource.gif).
  		///
  		/// Throws IllegalArgumentException if path doesn't have a leading slash or 
		/// is not an absolute URL.
};


Portlet_END


#endif //Portlet_PortletResponse_INCLUDED
