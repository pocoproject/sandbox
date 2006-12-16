/// Portlet.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletConfig_INCLUDED
#define Portlet_PortletConfig_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletContext_INCLUDED
#include "Portlet/PortletContext.h"
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


class CPP_PORTLET_DLL_API PortletConfig
	/// The PortletConfig interface provides the portlet with
	/// its configuration. The configuration holds information about the
	/// portlet that is valid for all users. The configuration is retrieved
	/// from the portlet definition in the deployment descriptor.
	/// The portlet can only read the configuration data.
	/// 
	/// The configuration information contains the portlet name, the portlet 
	/// initialization parameters, the portlet resource bundle and the portlet 
	/// application context.
{
public:
	virtual const std::string& getPortletName() const = 0;
  		/// Returns the name of the portlet.
  		/// 
  		/// The name may be provided via server administration, assigned in the
  		/// portlet application deployment descriptor with the portlet-name
  		/// tag.

	virtual const PortletContext& getPortletContext() const = 0;
  		/// Returns the PortletContext of the portlet application 
  		/// the portlet is in.

	//virtual const ResourceBundle& getResourceBundle(const std::locale& locale);
  		/// Gets the resource bundle for the given locale based on the
  		/// resource bundle defined in the deployment descriptor
  		/// with resource-bundle tag or the inlined resources
  		/// defined in the deployment descriptor.
  		///
  		/// Parameter   locale    the locale for which to retrieve the resource bundle

	virtual const std::string& getInitParameter(const std::string& name) const = 0;
  		/// Returns a String containing the value of the named initialization parameter, 
  		/// or null if the parameter does not exist.
  		///
  		/// Parameter name is a string specifying the name
  		///	of the initialization parameter
  		///
  		/// Throws IllegalArgumentException	if name is empty string.

	virtual const std::vector<std::string>& getInitParameterNames() const = 0;
  		/// Returns the names of the portlet initialization parameters as an 
  		/// Enumeration of String objects, or an empty Enumeration if the 
  		/// portlet has no initialization parameters. 
};


Portlet_END


#endif //Portlet_PortletConfig_INCLUDED
