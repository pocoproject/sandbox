/// PortletRequest.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletRequest_INCLUDED
#define Portlet_PortletRequest_INCLUDED

#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletMode_INCLUDED
#include "Portlet/PortletMode.h"
#endif
#ifndef Portlet_WindowState_INCLUDED
#include "Portlet/WindowState.h"
#endif
#ifndef Portlet_PortletPreferences_INCLUDED
#include "Portlet/PortletPreferences.h"
#endif
#ifndef Servlet_Object_INCLUDED
#include "Servlet/Object.h"
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
#ifndef STD_LOCALE_INCLUDED
#include <locale>
# define STD_LOCALE_INCLUDED
#endif


Portlet_BEGIN

class PortletSession;
class PortalContext;

class CPP_PORTLET_DLL_API PortletRequest
	/// The PortletRequest defines the base interface to provide client
	/// request information to a portlet. The portlet container uses two specialized
	/// versions of this interface when invoking a portlet, ActionRequest
	/// and RenderRequest. The portlet container creates these objects and 
	/// passes them as  arguments to the portlet's processAction and
	/// render methods.
{
public:
	static const std::string USER_INFO;
		/// Used to retrieve user information attributes with the 
		/// getAttribute call. The user information is returned 
		/// as a Map object. The portlet must define the 
		/// user information attribute it is interested in inside the 
		/// user-attribute section of the deployment descriptor.
		/// If an attribute is not supported
		/// by the current runtime system it will not show up in the user
		/// attribute map.
		/// If the user-attribute is supported by the runtime system, but not 
		/// defined for a particular user, then for that user the attribute 
		/// exists in the returned map and the attribute has a null value.
		/// 
		/// If the user-attribute is not defined for the current user it
		/// will not show up in the Map.
		/// 
		/// The value is javax.portlet.userinfo.

	static const std::string BASIC_AUTH;
		/// String identifier for Basic authentication. Value "BASIC".

	static const std::string FORM_AUTH;
		/// String identifier for Form based authentication. Value "FORM".

	static const std::string CLIENT_CERT_AUTH;
		/// String identifier for Certification based authentication. Value "CLIENT_CERT".

	static const std::string DIGEST_AUTH;
		/// String identifier for Digest based authentication. Value "DIGEST".

	virtual bool isWindowStateAllowed(const WindowState& state) const = 0;
		/// Returns true, if the given window state is valid
		/// to be set for this portlet in the context
		/// of the current request.
		///
		/// Parameter state indicates window state to checked

	virtual bool isPortletModeAllowed(const PortletMode& mode) const = 0;
		/// Returns true, if the given portlet mode is a valid
		/// one to set for this portlet  in the context
		/// of the current request.
		///
		/// Parameter mode indicates portlet mode to check

	virtual const PortletMode& getPortletMode() const = 0;
		/// Returns the current portlet mode of the portlet.

	virtual const WindowState& getWindowState() const = 0;
		/// Returns the current window state of the portlet.

	virtual const PortletPreferences& getPreferences() const = 0;
		/// Returns the preferences object associated with the portlet.

	virtual const PortletSession& getPortletSession() const = 0;
		/// Returns the current portlet session or, if there is no current session,
		/// creates one and returns the new session.
		///  
		/// Creating a new portlet session will result in creating
		/// a new HttpSession on which the portlet session is based on.

	virtual const PortletSession* getPortletSession (bool create) const = 0;
		/// Returns the current portlet session or, if there is no current session
		/// and the given flag is true, creates one and returns
		/// the new session.
		/// 
		/// If the given flag is false and there is no current
		/// portlet session, this method returns null.
		///  
		/// Creating a new portlet session will result in creating
		/// a new HttpSession on which the portlet session is based on.
		/// 
		/// Parameter create should be true to create a new session,
		/// false to return null if there is no current session
		/// Returns the portlet session

	virtual const std::string& getProperty(const std::string& name) const = 0; 
		/// Returns the value of the specified request property
		/// as a std::string. If the request did not include a property
		/// of the specified name, this method returns null.
		/// 
		/// A portlet can access portal/portlet-container specific properties 
		/// through this method and, if available, the
		/// headers of the HTTP client request.
		/// 
		/// This method should only be used if the 
		/// property has only one value. If the property might have
		/// more than one value, use {@link #getProperties}.
		/// 
		/// If this method is used with a multivalued
		/// parameter, the value returned is equal to the first value
		/// in the Enumeration returned by getProperties.
		///
		/// Parameter name	is a string specifying the property name
		///
		/// Returns a std::string containing the value of the requested
		///	property, or empty string if the request does not have a
		///	property of that name.
		///
		/// Throws IllegalArgumentException if name is null.
  
	virtual const std::vector<std::string>& getProperties(const std::string& name) const = 0; 
		/// Returns all the values of the specified request property
		/// as a Enumeration of std::string objects.
		/// 
		/// If the request did not include any propertys
		/// of the specified name, this method returns an empty
		/// vector. The property name is case insensitive. You can use
		/// this method with any request property.
		///
		/// Parameter name is a std::string specifying the property name
		///
		/// Returns a vector containing the values of the requested property. 
		/// If the request does not have any properties of that name return 
		/// an empty vector.
		///
		/// Throws IllegalArgumentException if name is null.

	virtual const std::vector<std::string>& getPropertyNames() const = 0;
		/// Returns a vector of all the property names
		/// this request contains. If the request has no
		/// properties, this method returns an empty vector.

	virtual const PortalContext& getPortalContext() const = 0;
		/// Returns the context of the calling portal.

	virtual const std::string& getAuthType() const = 0;
 		/// Returns the name of the authentication scheme used for the 
		/// connection between client and portal,
		/// for example, BASIC_AUTH, CLIENT_CERT_AUTH, 
		/// a custom one or null if there was no authentication.
		///
		/// Returns one of the static members BASIC_AUTH, FORM_AUTH, 
		/// CLIENT_CERT_AUTH, DIGEST_AUTH (suitable for == comparison) 
		///	indicating the authentication scheme, a custom one, or 
		///	empty string if the request was not authenticated.     

	virtual const std::string& getContextPath() const = 0;
	 	/// Returns the context path which is the path prefix associated with the deployed 
 		/// portlet application. If the portlet application is rooted at the
 		/// base of the web server URL namespace (also known as "default" context), 
 		/// this path must be an empty string. Otherwise, it must be the path the
 		/// portlet application is rooted to, the path must start with a '/' and 
 		/// it must not end with a '/' character.
 		/// 
		/// To encode a URL the { PortletResponse::encodeURL } method must be used.
 		///
 		/// Returns a std::string specifying the portion of the request URL that indicates 
		/// the context of the request

	virtual const std::string& getRemoteUser() const = 0;
		/// Returns the login of the user making this request, if the user 
		/// has been authenticated, or null if the user has not been authenticated.
		///
		/// Returns a std::string specifying the login of the user making this request, 
		/// or null if the user login is not known.

  //public java.security.Principal getUserPrincipal();
		/// Returns a java.security.Principal object containing the name of the 
		/// current authenticated user.
		///
		/// Returns a java.security.Principal containing
		///			the name of the user making this request, or
		///			null if the user has not been 
		///			authenticated.

	virtual bool isUserInRole(const std::string& role) const = 0;
		/// Returns a boolean indicating whether the authenticated user is 
		/// included in the specified logical "role".  Roles and role membership can be
		/// defined using deployment descriptors.  If the user has not been
		/// authenticated, the method returns false.
		///
		/// Parameter role is a std::string specifying the name of the role
		///
		/// Returns a boolean indicating whether the user making this request 
		/// belongs to a given role; false if the user has not been authenticated.

	virtual const Object& getAttribute(const std::string& name) const = 0;
		/// Returns the value of the named attribute as an Object,
		/// or null if no attribute of the given name exists. 
		/// 
		/// 
		/// In a distributed portlet web application the Object
		/// needs to be serializable.
		///
		/// Parameter name is a std::string specifying the name of 
		///	the attribute
		///
		/// Returns an Object containing the value of the attribute, 
		/// or null if the attribute does not exist.
		///
		/// Throws IllegalArgumentException if name is empty string.
  
	virtual const std::vector<std::string>& getAttributeNames() const = 0;
		/// Returns an Enumeration containing the
		/// names of the attributes available to this request. 
		/// This method returns an empty Enumeration
		/// if the request has no attributes available to it.
		/// 
		///
		/// Returns an Enumeration of strings containing the names 
		/// of the request attributes, or an empty vector if the request 
		/// has no attributes available to it.

	virtual const std::string& getParameter(const std::string& name) const = 0;
		/// Returns the value of a request parameter as a std::string,
		/// or null if the parameter does not exist. Request parameters
		/// are extra information sent with the request. The returned parameter 
		/// are "x-www-form-urlencoded" decoded.
		/// 
		/// Only parameters targeted to the current portlet are accessible.
		/// 
		/// This method should only be used if the 
		/// parameter has only one value. If the parameter might have
		/// more than one value, use {@link #getParameterValues}.
		/// 
		/// If this method is used with a multivalued
		/// parameter, the value returned is equal to the first value
		/// in the array returned by getParameterValues.
		///
		///
		///
		/// @param name 	a std::string specifying the 
		///			name of the parameter
		///
		/// Returns a std::string representing the 
		///			single value of the parameter
		///
		/// Throws IllegalArgumentException if name is empty string.

	virtual const std::vector<std::string>& getParameterNames() const = 0;
		/// Returns an Enumeration of std::string
		/// objects containing the names of the parameters contained
		/// in this request. If the request has 
		/// no parameters, the method returns an 
		/// empty Enumeration. 
		/// 
		/// Only parameters targeted to the current portlet are returned.
		///
		///
		/// Returns an Enumeration of std::string objects, each std::string 
		/// containing the name of a request parameter; or an empty vector 
		/// if the request has no parameters.

	virtual const std::vector<std::string>& getParameterValues(const std::string& name) const = 0;
		/// Returns an array of std::string objects containing 
		/// all of the values the given request parameter has, or 
		/// null if the parameter does not exist.
		/// The returned parameters are "x-www-form-urlencoded" decoded.
		/// 
		/// If the parameter has a single value, the array has a length
		/// of 1.
		///
		/// Parameter name is a std::string containing the name of 
		///			the parameter the value of which is requested
		///
		/// Returns an array of std::string objects 
		///			containing the parameter values.
		///
		/// Throws IllegalArgumentException if name is empty string.

	virtual const std::map<std::string, std::vector<std::string> >& getParameterMap() const = 0;
		/// Returns a Map of the parameters of this request.
		/// Request parameters are extra information sent with the request.  
		/// The returned parameters are "x-www-form-urlencoded" decoded.
		/// 
		/// The values in the returned Map are from type
		/// std::string array (std::string[]).
		/// 
		/// If no parameters exist this method returns an empty Map.
		///
		/// Returns an immutable Map containing parameter names as 
		/// keys and parameter values as map values, or an empty Map
		/// if no parameters exist. The keys in the parameter
		/// map are of type std::string. The values in the parameter map are of type
		/// std::string array (std::string[]).

	virtual bool isSecure() const = 0;
		/// Returns a boolean indicating whether this request was made 
		/// using a secure channel between client and the portal, such as HTTPS.
		/// 
		/// Returns  true, if the request was made using a secure channel.

	virtual void setAttribute(const std::string& name, const Object& o) = 0;
		/// Stores an attribute in this request.
		///
		/// Attribute names should follow the same conventions as
		/// package names. Names beginning with java.*,
		/// javax.*, and com.sun.* are
		/// reserved for use by Sun Microsystems.
		///
		/// If the value passed into this method is null, 
		/// the effect is the same as calling {@link #removeAttribute}.
		///
		/// Parameter name a std::string specifying the name of the attribute
		///
		/// Parameter o is the Object to be stored
		///
		/// Throws IllegalArgumentException if name is empty string.

	virtual void removeAttribute(std::string name) = 0;
		/// Removes an attribute from this request.  This method is not
		/// generally needed, as attributes only persist as long as the request
		/// is being handled.
		///
		/// Parameter name is a string specifying the name of the 
		/// attribute to be removed
		///
		/// Throws IllegalArgumentException if name is empty string.

	virtual const std::string& getRequestedSessionId() const = 0;
		/// Returns the session ID indicated in the client request.
		/// This session ID may not be a valid one, it may be an old 
		/// one that has expired or has been invalidated.
		/// If the client request
		/// did not specify a session ID, this method returns
		/// null.

	virtual bool isRequestedSessionIdValid() const = 0;
		/// Checks whether the requested session ID is still valid.
		/// and returns true if this request has an id for a valid session
		///	in the current session context, false otherwise.

	virtual const std::string& getResponseContentType() const = 0;
		/// Returns the portal preferred content type for the response.
		/// 
		/// The content type only includes the MIME type, not the
		/// character set.
		/// 
		/// Only content types that the portlet has defined in its
		/// deployment descriptor are valid return values for
		/// this method call. If the portlet has defined
		/// '*' or '* / *' as supported content
		/// types, these may also be valid return values.
		///
		/// Returns preferred MIME type of the response

	virtual const std::vector<std::string>& getResponseContentTypes() const = 0;
		/// Gets a list of content types which the portal accepts for the response.
		/// This list is ordered with the most preferable types listed first.
		/// 
		/// The content type only includes the MIME type, not the
		/// character set.
		/// 
		/// Only content types that the portlet has defined in its
		/// deployment descriptor are valid return values for
		/// this method call. If the portlet has defined
		/// '*' or '* / *' as supported content
		/// types, these may also be valid return values.


	virtual std::locale getLocale() const = 0;
		/// Returns the preferred Locale in which the portal will accept content.
		/// The Locale may be based on the Accept-Language header of the client.


	virtual const std::vector<std::locale>& getLocales() const = 0;
		/// Returns a vector of locale objects indicating, in decreasing
		/// order starting with the preferred locale in which the portal will
		/// accept content for this request.
		/// The Locales may be based on the Accept-Language header of the client.


	virtual const std::string& getScheme() const = 0;
		/// Returns the name of the scheme used to make this request.
		/// For example, http, https, or ftp.
		/// Different schemes have different rules for constructing URLs,
		/// as noted in RFC 1738.

	virtual const std::string& getServerName() const = 0;
		/// Returns the host name of the server that received the request.
		///
		/// Returns a std::string containing the name 
		///			of the server to which the request was sent

	virtual int getServerPort() const = 0;
		/// Returns the port number on which this request was received.
		///
		/// Returns an integer specifying the port number
};


Portlet_END


#endif //Portlet_PortletRequest_INCLUDED
