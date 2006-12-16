/// PortletRequestDispatcher.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletURL_INCLUDED
#define Portlet_PortletURL_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_RenderRequest_INCLUDED
#include "Portlet/RenderRequest.h"
#endif
#ifndef Portlet_RenderResponse_INCLUDED
#include "Portlet/RenderResponse.h"
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletRequestDispatcher 
	/// The PortletRequestDispatcher interface 
	/// defines an object that receives requests from the client
	/// and sends them to the specified resources (such as a servlet,
	/// HTML file, or JSP file) on the server. The portlet
	/// container creates the PortletRequestDispatcher object,
	/// which is used as a wrapper around a server resource located
	/// at a particular path or given by a particular name.
{ 
public:
	virtual void include(const RenderRequest& request, RenderResponse& response) = 0;
    		/// Includes the content of a resource (servlet, JSP page,
    		/// HTML file) in the response. In essence, this method enables 
    		/// programmatic server-side includes.
    		/// 
    		/// The included servlet cannot set or change the response status code
    		/// or set headers; any attempt to make a change is ignored.
    		///
    		/// Parameter request is a reference to RenderRequest object 
    		///	that contains the client request.
    		///
    		/// Parameter response is a reference to RenderResponse} object 
    		/// that contains the render response.
    		///
    		/// Throws PortletException if the included resource throws a ServletException, 
    		/// or other exceptions that are not Runtime or IOExceptions.
    		///
    		/// Throws IOException if the included resource throws this exception.
};


Portlet_END


#endif //Portlet_PortletRequestDispatcher_INCLUDED
