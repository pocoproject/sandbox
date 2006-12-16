/// PreferencesValidator.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PreferencesValidator_INCLUDED
#define Portlet_PreferencesValidator_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletPreferences_INCLUDED
#include "Portlet/PortletPreferences.h"
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PreferencesValidator
	/// The PreferencesValidator allows to validate the set of
	/// preferences of the associated portlet just before they are
	/// stored in the persistent store.
	/// 
	/// The portlet container invokes the validate method as 
	/// part of the invocation of the store method of the
	/// PortletPreferences.
{
public:
	virtual void validate(const PortletPreferences& preferences) const = 0;  
	  	/// If the preferences values are successfully validated the call to this method
  		/// must finish gracefully. Otherwise it must throw a ValidatorException.
  		///
  		/// Parameter preferences are preferences to validate.
  		///
  		/// Throws ValidatorException if the given preferences contains invalid settings
};


Portlet_END


#endif //Portlet_PreferencesValidator_INCLUDED
