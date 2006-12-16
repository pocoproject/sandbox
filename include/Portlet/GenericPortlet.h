/// GenericPortlet.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_GenericPortlet_INCLUDED
#define Portlet_GenericPortlet_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_Portlet_INCLUDED
#include "Portlet/Portlet.h"
#endif
#ifndef Portlet_PortletConfig_INCLUDED
#include "Portlet/PortletConfig.h"
#endif
#ifndef Portlet_ActionRequest_INCLUDED
#include "Portlet/ActionRequest.h"
#endif
#ifndef Portlet_ActionResponse_INCLUDED
#include "Portlet/ActionResponse.h"
#endif
#ifndef Portlet_RenderRequest_INCLUDED
#include "Portlet/RenderRequest.h"
#endif
#ifndef Portlet_RenderResponse_INCLUDED
#include "Portlet/RenderResponse.h"
#endif
#ifndef Portlet_PortletException_INCLUDED
#include "Portlet/PortletException.h"
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


class CPP_PORTLET_DLL_API GenericPortlet : public PPortlet, PortletConfig
	/// The GenericPortlet class provides a default implementation
	/// for the Portlet interface.
	/// 
	/// It provides an abstract class to be subclassed to create portlets. A 
	/// subclass of GenericPortlet should override at least
	/// one method, usually one of the following:
	/// 
	/// 	processAction, to handle action requests
	/// 	doView, to handle render requests when in VIEW mode
	/// 	doEdit, to handle render requests when in EDIT mode
	/// 	doHelp, to handle render request when in HELP mode
	/// 	init and destroy, to manage resources that are held for the life of 
	///		the servlet
	/// 
	/// 
	/// Normally there is no need to override the render or the doDispatch 
	/// methods. Render handles render requests setting the title of the 
	/// portlet in the response and invoking doDispatch. doDispatch dispatches 
	/// the request to one of the doView, doEdit or doHelp method depending on 
	/// the portlet mode indicated in the request.
	/// 
	/// Portlets typically run on multithreaded servers, so please note that a 
	/// portlet must handle concurrent requests and be careful to synchronize 
	/// access to shared resources.  Shared resources include in-memory data 
	/// such as  instance or class variables and external objects  such as 
	/// files, database connections, and network  connections.
{
public:
	GenericPortlet();
		/// Constructor.

	virtual ~GenericPortlet();
		/// Destructor.

	virtual void init (PortletConfig* pConfig);
  		/// Called by the portlet container to indicate to a portlet that the 
  		/// portlet is being placed into service.
  		/// 
  		/// The default implementation just stores the PortletConfig
  		/// object.
  		/// The portlet container calls the init
  		/// method exactly once after instantiating the portlet.
  		/// The init method must complete successfully
  		/// before the portlet can receive any requests.
  		///
  		/// The portlet container cannot place the portlet into service
  		/// if the init method does one of the following:
  		/// 
  		/// 	it throws a PortletException
  		/// 	it does not return within a time period defined by the Web server
  		///
  		/// Parameter pConfig is a pointer to PortletConfig object 
  		///	containing the portlet configuration and initialization parameters.
		/// portlet takes ownership of supplied object.
  		///
  		/// Throws PortletException if an exception has occurred that
  		///	interferes with the portlet normal operation.
  		/// Throws UnavailableException if the portlet cannot perform the 
		/// initialization at this time.

	virtual void init();
  		/// This method is called by GenericPortlet::init(PortletConfig* pConfig). 
  		///
  		/// Throws PortletException if an exception has occurred that
  		///	interferes with the portlet normal operation.
  		/// Throws UnavailableException if the portlet is unavailable to perform init.
  
	virtual void processAction(const ActionRequest& request, ActionResponse& response);
  		/// Called by the portlet container to allow the portlet to process
  		/// an action request. This method is called if the client request was
  		/// originated by a URL created (by the portlet) with the 
  		/// RenderResponse.createActionURL() method.
  		/// 
  		/// The default implementation throws an exception.
  		///
  		/// Parameter request is the action request
  		/// Parameter response is the action response
  		/// Throws PortletException if the portlet cannot fulfilling the request
  		/// Throws UnavailableException if the portlet is unavailable to process	
  		/// the action at this time.
  		/// Throws PortletSecurityException if the portlet cannot fullfill this  
  		/// request because of security reasons.
  		/// Throws IOException if the streaming causes an I/O problem.
  
	virtual void render (const RenderRequest& request, RenderResponse& response);
   		/// The default implementation of this method sets the title 
  		/// using the getTitle method and invokes the
  		/// doDispatch method.
  		/// 
  		/// Parameter request is the render request.
  		/// Parameter response is the render response.
  		///
  		/// Throws PortletException if the portlet cannot fulfilling the request.
  		/// Throws UnavailableException if the portlet is unavailable	
  		/// to perform render at this time.
  		/// Throws PortletSecurityException if the portlet cannot fullfill 
  		/// this request because of security reasons.
  		/// ThrowsIOException if the streaming causes an I/O problem.

protected:
	virtual std::string getTitle(const RenderRequest& request) const;
  		/// Used by the render method to get the title.
  		/// 
  		/// The default implementation gets the title from the ResourceBundle
  		/// of the PortletConfig of the portlet. The title is retrieved
  		/// using the 'javax.portlet.title' resource name.
  		/// 
  		/// Portlets can overwrite this method to provide dynamic
  		/// titles (e.g. based on locale, client, and session information).
  		/// Examples are:
  		/// 
  		/// 	language-dependant titles for multi-lingual portals
  		/// 	shorter titles for WAP phones
  		/// 	the number of messages in a mailbox portlet

	virtual void doDispatch (const RenderRequest& request, RenderResponse& response);
  		/// The default implementation of this method routes the render request
  		/// to a set of helper methods depending on the current portlet mode the
  		/// portlet is currently in.
  		/// These methods are:
  		/// 
  		/// 	doView for handling view requests
  		/// 	doEdit for handling edit requests
  		/// 	doHelp for handling help requests
  		/// 
  		/// If the window state of this portlet is minimized, this
  		/// method does not invoke any of the portlet mode rendering methods.
  		/// 
  		/// For handling custom portlet modes the portlet should override this
  		/// method.
  		///
  		/// Parameter request is reference to the render request
  		/// Parameter response is reference to the render response
  		///
  		/// Throws PortletException if the portlet cannot fulfill the request.
  		/// Throws UnavailableException if the portlet is unavailable to perform 
		/// render at this time.
  		/// Throws PortletSecurityException if the portlet cannot fullfill this 
		/// request because of security reasons.
  		/// ThrowsIOException if the streaming causes an I/O problem.

	virtual void doView (const RenderRequest& request, RenderResponse& response);
  		/// Helper method to serve up the mandatory view mode.
  		/// 
  		/// The default implementation throws an exception.
  		///
  		/// Parameter request is the reference to render request
  		/// Parameter response is the reference to render response
  		///
  		/// Throws PortletException if the portlet cannot fulfilling the request
  		/// Throws UnavailableException if the portlet is unavailable to perform 
		/// render at this time.
  		/// Throws PortletSecurityException if the portlet cannot fullfill this 
		/// request because of security reasons.
  		/// Throws IOException if the streaming causes an I/O problem

	virtual void doEdit(const RenderRequest& request, RenderResponse& response);
  		/// Helper method to serve up the edit mode.
  		/// 
  		/// The default implementation throws an exception.
  		///
  		/// Parameter request is the reference to render request.
  		/// Parameter response is the reference to the render response.
  		///
  		/// Throws PortletException if the portlet cannot fulfilling the request
  		/// Throws UnavailableException  if the portlet is unavailable to perform 
		/// render at this time.
  		/// Throws PortletSecurityException if the portlet cannot fullfill this 
		/// request because of security reasons.
  		/// Throws IOException if the streaming causes an I/O problem.

	virtual void doHelp (const RenderRequest& request, RenderResponse& response);
  		/// Helper method to serve up the help mode.
  		/// 
  		/// The default implementation throws an exception.
  		///
  		/// Parameter request is refernce to the render request.
  		/// Parameter response is refernce to the render response.
  		///
  		/// Throws PortletException if the portlet cannot fulfilling the request
  		/// Throws  UnavailableException if the portlet is unavailable to perform 
		/// render at this time.
  		/// Throws  PortletSecurityException if the portlet cannot fullfill this 
		/// request because of security reasons.
  		/// Throws IOException if the streaming causes an I/O problem

	virtual const PortletConfig& getPortletConfig();
  		/// Returns the PortletConfig object of this portlet.
  
	virtual void destroy();
  		/// Called by the portlet container to indicate to a portlet that the portlet 
  		/// is being taken out of service.
  		/// 
  		/// The default implementation does nothing.

	// PortletConfig implementation

	virtual const std::string& getPortletName();
  		/// Returns the name of this portlet.

	virtual const PortletContext& getPortletContext();
  		/// Returns the PortletContext of the portlet application 
  		/// the portlet is in.

/***
  public ResourceBundle getResourceBundle(const std::locale& locale)
  		/// Gets the resource bundle for the given locale based on the
  		/// resource bundle defined in the deployment descriptor
  		/// with resource-bundle tag or the inlined resources
  		/// defined in the deployment descriptor.
  {
  	return config.getResourceBundle(locale);
  }
***/

	virtual const std::string& getInitParameter(const std::string& name);
  		/// Returns a string containing the value of the named initialization parameter, 
  		/// or null if the parameter does not exist.
  		///
  		/// Parameter name is a string specifying the name
  		///	of the initialization parameter
  		///
  		/// Throws IllegalArgumentException	if name is empty.

	virtual const std::vector<std::string>& getInitParameterNames();
  		/// Returns the names of the portlet initialization parameters as a 
  		/// string vector, or an empty vector if the portlet has no
  		/// initialization parameters.

protected:
	void checkConfig() const;

private:
	PortletConfig* _pConfig;
};


/// inlines

inline void GenericPortlet::checkConfig() const
{
	if(0 == _pConfig)
		throw PortletException("init(PortletConfig*) must be called "
			"in order for portlet to be ready for service.");
}


Portlet_END


#endif // Portlet_GenericPortlet_INCLUDED
