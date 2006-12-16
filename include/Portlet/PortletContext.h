/// PortletContext.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_PortletContext_INCLUDED
#define Portlet_PortletContext_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletRequestDispatcher_INCLUDED
#include "Portlet/PortletRequestDispatcher.h"
#endif
#ifndef Servlet_ServletInputStream_INCLUDED
#include "Servlet/ServletInputStream.h"
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
#ifndef STD_SET_INCLUDED
#include <set>
# define STD_SET_INCLUDED
#endif

Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletContext
		/// The PortletContext interface defines a portlet view
		/// of the portlet container.
		/// The PortletContext also makes resources available
		/// to the portlet. Using the context, a portlet can access
		/// the portlet log, and obtain URL references to resources.
		/// 
		/// There is one context per "portlet application" per Java Virtual Machine.  (A
		/// "portlet application" is a collection of portlets, servlets, and content installed
		/// under a specific subset of the server URL namespace, such as /catalog.
		/// They are possibly installed via a .war file.)
		/// As a web application, a portlet application also has a servlet context.
		/// The portlet context leverages most of its functionality from the
		/// servlet context of the portlet application.
		/// 
		/// Attibutes stored in the context are global for all users and all
		/// components in the portlet application.
		/// 
		/// In the case of a web
		/// application marked "distributed" in its deployment descriptor, there will
		/// be one context instance for each virtual machine.  In this situation, the
		/// context cannot be used as a location to share global information (because
		/// the information is not truly global). Use an external resource, such as
		/// a database to achieve sharing on a global scope.
{
public:
	typedef Servlet::ServletInputStream InputStream;

	virtual const std::string& getServerInfo() const = 0;
  		/// Returns the name and version of the portlet container in which the
  		/// portlet is running.
  		///
  		/// The form of the returned string is containername/versionnumber.

	virtual PortletRequestDispatcher& getRequestDispatcher(const std::string& path) = 0;
  		/// Returns a {PortletRequestDispatcher} object that acts
  		/// as a wrapper for the resource located at the given path.
  		/// A PortletRequestDispatcher object can be used include the
  		/// resource in a response. The resource can be dynamic or static.
  		/// 
  		/// The pathname must begin with a slash ( / ) and is interpreted as relative
  		/// to the current context root.
  		/// 
  		/// This method returns null if the PortletContext
  		/// cannot return a PortletRequestDispatcher
  		/// for any reason.
  		/// 
  		/// Parameter path is a String specifying the pathname
  		///  to the resource.

	virtual PortletRequestDispatcher& getNamedDispatcher(const std::string& name) = 0;
  		/// Returns a {PortletRequestDispatcher} object that acts
  		/// as a wrapper for the named servlet.
  		///
  		/// Servlets (and also JSP pages) may be given names via server 
  		/// administration or via a web application deployment descriptor.
  		///
  		/// This method returns null if the 
  		/// PortletContext cannot return a 
  		/// PortletRequestDispatcher for any reason.
  		///
  		/// Parameter name is a string specifying the name
  		///	of a servlet to be wrapped.

	virtual InputStream& getResourceAsStream (const std::string& path) = 0;
  		/// Returns the resource located at the given path as an InputStream object.
  		/// The data in the InputStream can be of any type or length. The method returns 
  		/// null if no resource exists at the given path.
  		/// 
  		/// In order to access protected resources the path has to be prefixed with 
  		/// /WEB-INF/ (for example /WEB-INF/myportlet/myportlet.jsp). 
  		/// Otherwise, the direct path is used
  		/// (for example /myportlet/myportlet.jsp).
  		///
  		/// Parameter path is the path to the resource.

	virtual int getMajorVersion() const = 0;
  		/// Returns the major version of the Portlet API that this portlet
  		/// container supports.  

	virtual int getMinorVersion() const = 0;
  		/// Returns the minor version of the Portlet API that this portlet
  		/// container supports.

	virtual const std::string& getMimeType(const std::string& file) const = 0;
  		/// Returns the MIME type of the specified file, or null if 
  		/// the MIME type is not known. The MIME type is determined
  		/// by the configuration of the portlet container and may be specified
  		/// in a web application deployment descriptor. Common MIME
  		/// types are text/html and image/gif.
  		///
  		///
  		/// Parameter file is a string specifying the name of a file.
  
	virtual const std::string& getRealPath(const std::string& path) const = 0;
  		/// Returns a string containing the real path 
  		/// for a given virtual path. For example, the path /index.html
  		/// returns the absolute file path of the portlet container file system.
  		///
  		/// The real path returned will be in a form
  		/// appropriate to the computer and operating system on
  		/// which the portlet container is running, including the
  		/// proper path separators. This method returns null
  		/// if the portlet container cannot translate the virtual path
  		/// to a real path for any reason.
  		///
  		/// Parameter path 	a String specifying a virtual path.
    
	virtual const std::set<std::string> getResourcePaths(const std::string& path) const = 0;
  		/// Returns a directory-like listing of all the paths to resources within 
  		/// the web application longest sub-path of which 
  		/// matches the supplied path argument. Paths indicating subdirectory paths 
  		/// end with a slash (/). The returned paths are all 
  		/// relative to the root of the web application and have a leading slash. 
  		/// For example, for a web application 
  		/// containing
  		/// 
  		/// /welcome.html
  		/// /catalog/index.html
  		/// /catalog/products.html
  		/// /catalog/offers/books.html
  		/// /catalog/offers/music.html
  		/// /customer/login.jsp
  		/// /WEB-INF/web.xml
  		/// /WEB-INF/classes/com.acme.OrderPortlet.class,
  		/// 
  		///
  		/// getResourcePaths("/") returns 
  		/// {"/welcome.html", "/catalog/", "/customer/", "/WEB-INF/"}
  		/// getResourcePaths("/catalog/") returns 
  		/// {"/catalog/index.html", "/catalog/products.html", "/catalog/offers/"}.
  		///
		/// Parameter path is the partial path used to match the resources 
		/// (must start with a slash).  
    
	virtual const std::string& getResource(const std::string& path) const = 0;
  		/// Returns a URL to the resource that is mapped to a specified
  		/// path. The path must begin with a slash (/) and is interpreted
  		/// as relative to the current context root.
  		///
  		/// This method allows the portlet container to make a resource 
  		/// available to portlets from any source. Resources 
  		/// can be located on a local or remote
  		/// file system, in a database, or in a .war file. 
  		///
  		/// The portlet container must implement the URL handlers
  		/// and URLConnection objects that are necessary
  		/// to access the resource.
  		///
  		/// This method returns empty string
  		/// if no resource is mapped to the pathname.
  		///
  		/// Some containers may allow writing to the URL returned by
  		/// this method using the methods of the URL class.
  		///
  		/// The resource content is returned directly, so be aware that 
  		/// requesting a .jsp page returns the JSP source code.
  		/// Use a RequestDispatcher instead to include results of 
  		/// an execution.
  		///
  		/// This method has a different purpose than
  		/// java.lang.Class.getResource,
  		/// which looks up resources based on a class loader. This
  		/// method does not use class loaders.
  		/// 
  		/// Parameter path is a string specifying the path to the resource
  		///
  		/// Throws MalformedURLException if the pathname is not given in 
  		/// the correct form. 

	virtual const Object* getAttribute(const std::string& name) const = 0;
  		/// Returns the portlet container attribute with the given name, 
  		/// or null if there is no attribute by that name.
  		/// An attribute allows a portlet container to give the
  		/// portlet additional information not
  		/// already provided by this interface.
  		/// A list of supported attributes can be retrieved using
  		/// getAttributeNames.
  		///
  		/// The attribute is returned as an Object or a subclass.
  		///
  		/// Parameter name is a string specifying the name 
  		///	of the attribute.
  		///
  		/// Throws IllegalArgumentException	if name is empty string.

	virtual const std::vector<std::string> getAttributeNames() const = 0;
  		/// Returns a string vector containing the attribute names 
  		/// available within this portlet context, or an emtpy
  		/// vector if no attributes are available. Use the
  		/// {getAttribute} method with an attribute name
  		/// to get the value of an attribute.

	virtual const std::string getInitParameter(const std::string& name) = 0;
  		/// Returns a String containing the value of the named context-wide 
  		/// initialization parameter, or null if the parameter does not exist.
  		/// This method provides configuration information which may be useful for 
  		/// an entire "portlet application".
  		///
  		/// Parameter name is a string containing the name of the
  		/// requested parameter.
  		///
  		/// Throws IllegalArgumentException	 if name is empty string. 

	virtual const std::vector<std::string>& getInitParameterNames() const =0;
  		/// Returns the names of the context initialization parameters as a
  		/// vector of strings, or an empty vector if the context 
  		/// has no initialization parameters. 

	virtual void log(const std::string& msg) = 0;
  		/// Writes the specified message to a portlet log file, usually an event log.
  		/// The name and type of the portlet log file is specific to the portlet container.
  		/// 
  		/// This method mapps to the ServletContext.log method.
  		/// The portlet container may in addition log this message in a
  		/// portlet container specific log file.
  		///
  		/// Parameter msg is a string specifying the message to be written to the log file.

	virtual void log(const std::string& message, const Poco::Exception& ex) = 0;
  		/// Writes an explanatory message and a stack trace for a given 
  		/// Throwable exception to the portlet log file.
  		/// The name and type of the portlet log file is specific to the 
  		/// portlet container, usually an event log.
  		/// 
  		/// This method is mapped to the ServletContext.log method.
  		/// The portlet container may in addition log this message in a
  		/// portlet container specific log file.
  		///
  		/// Parameter message a string that describes the error or exception
  		/// Parameter ex is the exception

	virtual void removeAttribute(const std::string& name) = 0;
  		/// Removes the attribute with the given name from the portlet context.
  		/// After removal, subsequent calls to
  		/// {getAttribute} to retrieve the attribute's value
  		/// will return null.
  		///
  		/// Parameter name	a String specifying the name of the attribute to be removed
  		///
  		/// Throws IllegalArgumentException	if name is empty string.


	virtual void setAttribute(const std::string& name, const Object& object) = 0;
  		/// Binds an object to a given attribute name in this portlet context.
  		/// If the name specified is already used for an attribute, this method 
  		/// removes the old attribute and binds the name to the new attribute.
  		/// 
  		/// If a null value is passed, the effect is the same as calling 
  		/// removeAttribute().
  		/// 
  		/// Attribute names should follow the same convention as package
  		/// names. The Java Portlet API specification reserves names
  		/// matching java.*, javax.*, and
  		/// sun.*.
  		///
  		/// Parameter name is	a String specifying the name 
  		///			of the attribute
  		/// Parameter object is	the Object representing the
  		///			attribute to be bound
  		///
  		/// Throws IllegalArgumentException if name is empty string.
   
	virtual const std::string& getPortletContextName() const = 0;
  		/// Returns the name of this portlet application correponding to this PortletContext as specified 
  		/// in the web.xml deployment descriptor for this web application by the 
  		/// display-name element.

};


Portlet_END


#endif //Portlet_PortletContext_INCLUDED
