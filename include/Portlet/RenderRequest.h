/// RenderRequest.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_RenderRequest_INCLUDED
#define Portlet_RenderRequest_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletRequest_INCLUDED
#include "Portlet/PortletRequest.h"
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API RenderRequest : public PortletRequest
	/// The RenderRequest represents the request sent to the portlet
	/// to handle a render.
	/// It extends the PortletRequest interface to provide render request
	/// information to portlets.
	/// 
	/// The portlet container creates a RenderRequest object and
	/// passes it as argument to the portlet's render method.
{
};


Portlet_END


#endif //Portlet_RenderRequest_INCLUDED
