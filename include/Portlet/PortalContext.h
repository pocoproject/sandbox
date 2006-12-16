/// PortalContext.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortalContext_INCLUDED
#define Portlet_PortalContext_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
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


class CPP_PORTLET_DLL_API PortalContext
	/// The PortalContext interface gives the portlet
	/// the ability to retrieve information about the portal calling this portlet.
	/// 
	/// The portlet can only read the PortalContext data.
{
public:
	virtual const std::string& getProperty(const std::string& name) const = 0;
  		/// Returns the portal property with the given name, 
  		/// or a null if there is 
  		/// no property by that name.
  		///
  		/// Parameter name is property name
  		///
  		/// Throws IllegalArgumentException	if name is empty string.
   
	virtual const std::vector<std::string>& getPropertyNames() const = 0;
  		/// Returns all portal property names, or an empty 
  		/// vector if there are no property names.

	virtual const std::vector<std::string>& getSupportedPortletModes() const = 0;
  		/// Returns all supported portlet modes by the portal
  		/// as an enumertation of PorltetMode objects.
  		/// 
  		/// The portlet modes must at least include the
  		/// standard portlet modes EDIT, HELP, VIEW.

	virtual const std::vector<std::string>& getSupportedWindowStates() const = 0;
  		/// Returns all supported window states by the portal
  		/// as an enumertation of WindowState objects.
  		/// 
  		/// The window states must at least include the
  		/// standard window states  MINIMIZED, NORMAL, MAXIMIZED.

	virtual const std::string& getPortalInfo() const = 0;
  		/// Returns information about the portal like vendor, version, etc.
  		/// 
  		/// The form of the returned string is servername/versionnumber. For 
  		/// example, the reference implementation Pluto may return the string 
  		/// Pluto/1.0.
  		/// 
  		/// The portlet container may return other optional information  after the 
  		/// primary string in parentheses, for example, Pluto/1.0 
  		/// (JDK 1.3.1; Windows NT 4.0 x86).
};


Portlet_END


#endif //Portlet_PortalContext_INCLUDED
