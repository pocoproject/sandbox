/// PortletSessionUtil.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_PortletSessionUtil_INCLUDED
#define Portlet_PortletSessionUtil_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletSessionUtil
	/// The PortletSessionUtil class helps identify and decode
	/// attributes in the PORTLET_SCOPE scope of the PortletSession
	/// when accessed through the HttpSession an from within calls to methods
	/// of the HttpSessionBindingListener interface.
{

	static std::string& decodeAttributeName(std::string& name);
  		/// Returns the attribute name of an attribute in the 
  		/// PORTLET_SCOPE. If the attribute is in the
  		/// APPLICATION_SCOPE it returns the attribute name unchanged.
  		///
  		/// Parameter name is a string specifying the name of the
  		/// encoded portlet attribute

	static int decodeScope(const std::string& name);
  		/// Returns the portlet attribute scope from an encoded portlet
  		/// attribute.
  		/// Possible return values are:
  		/// 
  		/// 	PortletSession.APPLICATION_SCOPE
  		/// 	PortletSession.PORTLET_SCOPE
  		///
  		/// Parameter name is a string specifying the name of the
  		/// encoded portlet attribute

private:
	static const std::string PORTLET_SCOPE_NAMESPACE;
};


Portlet_END


#endif // Portlet_PortletSessionUtil_INCLUDED
