/// PortletSession.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletSession_INCLUDED
#define Portlet_PortletSession_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletContext_INCLUDED
#include "Portlet/PortletContext.h"
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


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletSession
	/// The PortletSession interface provides a way to identify a user
	/// across more than one request and to store transient information about that user.
	/// 
	/// A PortletSession is created per user client per portlet application.
	/// 
	/// A portlet can bind an object attribute into a PortletSession by name.
	/// The PortletSession interface defines two scopes for storing objects:
	/// 
	///		APPLICATION_SCOPE
	///		PORTLET_SCOPE
	/// 
	/// All objects stored in the session using the APPLICATION_SCOPE 
	/// must be available to all the portlets, servlets and 
	/// JSPs that belongs to the same portlet application and that handles a 
	/// request identified as being a part of the same session.
	/// Objects stored in the session using the PORTLET_SCOPE must be
	/// available to the portlet during requests for the same portlet window
	/// that the objects where stored from. Attributes stored in the
	/// PORTLET_SCOPE are not protected from other web components
	/// of the portlet application. They are just conveniently namespaced.
	/// 
	/// The portlet session is based on the HttpSession. Therefore all
	/// HttpSession listeners do apply to the portlet session and
	/// attributes set in the portlet session are visible in the HttpSession
	/// and vice versa.
{
public:
	static const int APPLICATION_SCOPE;
  		/// This constant defines an application wide scope for the session attribute.
  		/// APPLICATION_SCOPE session attributes enable Portlets 
  		/// within one portlet application to share data.
  		/// 
  		/// Portlets may need to prefix attributes set in this scope with some
  		/// ID, to avoid overwriting each other's attributes in the
  		/// case where two portlets of the same portlet definition
  		/// are created.
  		/// 
  		/// Value: 0x01
  
	static const int PORTLET_SCOPE;
  		/// This constant defines the scope of the session attribute to be
  		/// private to the portlet and its included resources. 
  		/// 
  		/// Value: 0x02

	virtual const Object* getAttribute(const std::string& name) const = 0;
  		/// Returns the object bound with the specified name in this session
  		/// under the PORTLET_SCOPE, or null if no 
  		/// object is bound under the name in that scope.
  		///
  		/// Parameter name is a string specifying the name of the object
  		///
  		///
  		/// Throws IllegalStateException if this method is called on an
  		///					invalidated session.
  		/// Throws IllegalArgumentException if name is null.

	virtual const Object* getAttribute(const std::string& name, int scope) const = 0;
  		/// Returns the object bound with the specified name in this session, 
  		/// or null if no object is bound under the name in the given scope.
  		///
  		/// Parameter name is a string specifying the name of the object
  		/// Parameter scope is session scope of this attribute
  		///
  		/// Throws IllegalStateException if this method is called on an
  		///					invalidated session
  		/// Throws IllegalArgumentException if name is null.
  
	virtual const std::vector<std::string>& getAttributeNames() const = 0;
  		/// Returns a vector of strings containing the names of 
  		/// all the objects bound to this session under the PORTLET_SCOPE, or an
  		/// empty Enumeration if no attributes are available.
  		///
  		/// Throws IllegalStateException if this method is called on an
  		///	invalidated session.   
  
	virtual const std::vector<std::string>& getAttributeNames(int scope) const = 0;
 		/// Returns a vector of strings containing the names of 
  		/// all the objects bound to this session in the given scope, or an
  		/// empty Enumeration if no attributes are available in the
  		/// given scope.
  		///
  		/// Parameter scope is session scope of the attribute names
  		///
  		///
  		/// throws IllegalStateException	if this method is called on an
  		///	invalidated session.
  
	virtual long getCreationTime() const = 0;
    	/// Returns the time when this session was created, measured in 
  		/// milliseconds since midnight January 1, 1970 GMT.  
  		///
  		/// Throws IllegalStateException if this method is called on an
  		///	invalidated session.

  	virtual const std::string& getId() const = 0;
  		/// Returns a string containing the unique identifier assigned to this session. 
  		///
  		/// Returns	a string specifying the identifier assigned to this session
  
	virtual long getLastAccessedTime() const = 0;
  		/// Returns the last time the client sent a request associated with this session, 
  		/// as the number of milliseconds since midnight January 1, 1970 GMT.  
  		///
  		/// Actions that a portlet takes, such as getting or setting
  		/// a value associated with the session, do not affect the access
  		/// time.

	virtual int getMaxInactiveInterval() const = 0;
  		/// Returns the maximum time interval, in seconds, for which the portlet container 
  		/// keeps this session open between client accesses. After this interval, 
  		/// the portlet container invalidates the session.  The maximum time 
  		/// interval can be set
  		/// with the setMaxInactiveInterval method.
  		/// A negative time indicates the session should never timeout. 

	virtual void invalidate() const = 0;
  		/// Invalidates this session (all scopes) and unbinds any objects bound to it.  
  		/// 
  		/// Invalidating the portlet session will result in invalidating the underlying
  		/// HttpSession
  		///
  		/// Throws IllegalStateException if this method is called on a
  		///					session which has already been invalidated

	virtual bool isNew() const = 0;
   		/// Returns true if the client does not yet know about the session or 
  		/// if the client chooses not to join the session. 
  		///
  		/// Throws IllegalStateException if this method is called on a
  		///	session which has already been invalidated.
  
	virtual void removeAttribute(const std::string& name) = 0;
  		/// Removes the object bound with the specified name under
  		/// the PORTLET_SCOPE from
  		/// this session. If the session does not have an object
  		/// bound with the specified name, this method does nothing.
  		/// 
  		/// Throws IllegalStateException if this method is called on a
  		/// session which has been invalidated.
  		/// Throws IllegalArgumentException if name is empty string.
  
	virtual void removeAttribute(const std::string& name, int scope) = 0;
  		/// Removes the object bound with the specified name and the given scope from
  		/// this session. If the session does not have an object
  		/// bound with the specified name, this method does nothing.
  		/// 
  		/// Parameter name   the name of the object to be
  		///               removed from this session
  		/// Parameter scope  session scope of this attribute
  		///
  		/// ThrowsIllegalStateException if this method is called on a
  		/// session which has been invalidated
  		/// Throws IllegalArgumentException if name is empty string.
  
	virtual void setAttribute(const std::string& name, const Object& value) = 0;
  		/// Binds an object to this session under the PORTLET_SCOPE, using the name specified.  
  		/// If an object of the same name in this scope is already bound to the session,
  		/// that object is replaced.
  		///
  		/// After this method has been executed, and if the new object
  		/// implements HttpSessionBindingListener, the container calls 
  		/// HttpSessionBindingListener.valueBound. The container then   
  		/// notifies any HttpSessionAttributeListeners in the web 
  		/// application.
  		/// If an object was already bound to this session 
  		/// that implements HttpSessionBindingListener, its 
  		/// HttpSessionBindingListener.valueUnbound method is called.
  		///
  		/// If the value is empty string, this has the same effect as calling 
  		/// removeAttribute().
  		///
  		/// Parameter name is the name to which the object is bound under
  		/// the PORTLET_SCOPE; this cannot be empty string.
  		/// Parameter value	is the object to be bound
  		///
  		/// Throws IllegalStateException if this method is called on a
  		///	session which has been invalidated.
  		/// Throws IllegalArgumentException if name is empty string.

	virtual void setAttribute(const std::string& name, const Object& value, int scope) = 0;
  		/// Binds an object to this session in the given scope, using the name specified.  
  		/// If an object of the same name in this scope is already bound to the session,
  		/// that object is replaced.
  		///
  		/// After this method has been executed, and if the new object
  		/// implements HttpSessionBindingListener,
  		/// the container calls 
  		/// HttpSessionBindingListener.valueBound. The container then   
  		/// notifies any HttpSessionAttributeListeners in the web 
  		/// application.
  		/// If an object was already bound to this session 
  		/// that implements HttpSessionBindingListener, its 
  		/// HttpSessionBindingListener.valueUnbound method is called.
  		///
  		/// If the value is null, this has the same effect as calling 
  		/// removeAttribute().
  		///
  		///
  		/// Parameter name is the name to which the object is bound;
  		///				this cannot be empty string.
  		/// Parameter value is the object to be bound
  		/// Parameter scope  is the session scope of this attribute
  		///
  		/// Throws IllegalStateException if this method is called on a
  		///	session which has been invalidated
  		/// @Throws IllegalArgumentException if name is empty string.

	virtual void setMaxInactiveInterval(int interval) = 0;
   		/// Specifies the time, in seconds, between client requests, before the 
  		/// portlet container invalidates this session. A negative time
  		/// indicates the session should never timeout.
  		///
  		/// Parameter interval is an integer specifying the number
  		/// of seconds 

	virtual const PortletContext& getPortletContext() const = 0;
  		/// Returns the portlet application context associated with this session.
};


Portlet_END


#endif //Portlet_PortletSession_INCLUDED
