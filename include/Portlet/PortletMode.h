/// PortletMode.h


/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_PortletMode_INCLUDED
#define Portlet_PortletMode_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif

Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletMode
	/// The PortletMode class represents
	/// the possible modes that a portlet can assume.
	/// 
	/// A portlet mode indicates the function a portlet is performing.
	/// Normally, portlets perform different tasks and create different
	/// content depending on the function they are currently performing.
	/// When invoking a portlet, the portlet container provides the
	/// current portlet mode to the portlet.
	/// 
	/// Portlets can programmatically change their portlet
	/// mode when processing an action request.
	/// 
	/// This class defines the default portlet modes EDIT, HELP, VIEW.
	/// Additional portlet modes may be defined by calling the constructor
	/// of this class. If a portal/portlet-container does not support a 
	/// custom portlet mode defined in the portlet application deployment descriptor, 
	/// the custom portlet mode will be ignored by the portal/portlet container.
{
public:
  static const PortletMode VIEW;
		/// The expected functionality for a portlet in VIEW portlet mode 
		/// is to generate markup reflecting the current state of the portlet. 
		/// For example, the VIEW portlet mode of a portlet may 
		/// include one or more screens that the user can navigate and interact 
		/// with, or it may consist of static content that does not require any 
		/// user interaction.
		/// 
		/// This mode must be supported by the portlet.
		/// 
		/// The string value for this mode is "view".

  static const PortletMode EDIT;
		/// Within the EDIT portlet mode, a portlet should provide 
		/// content and logic that lets a user customize the behavior of the portlet. 
		/// The EDIT portlet mode may include one or more screens among which 
		/// users can navigate to enter their customization data.
		/// 
		/// Typically, portlets in EDIT portlet mode will 
		/// set or update portlet preferences.
		/// 
		/// This mode is optional.
		/// 
		/// The string value for this mode is "edit".

  static const PortletMode HELP;
		/// When in HELP portlet mode, a portlet should provide help 
		/// information about the portlet. This help information could be 
		/// a simple help screen explaining the entire portlet in
		/// coherent text or it could be context-sensitive help.
		/// 
		/// This mode is optional.
		/// 
		/// The string value for this mode is "help".

  PortletMode(const std::string& name);
		/// Creates a new portlet mode with the given name.
		/// 
		/// Upper case letters in the name are converted to
		/// lower case letters.
		///
		/// Parameter name is the name of the portlet mode

	const std::string& toString() const;
		/// Returns a String representation of this portlet mode.
		/// Portlet mode names are always lower case names.
		///
		/// Returns the string representation of this portlet mode

	bool equals(const PortletMode& mode);
		/// Compares the specified object with this portlet mode
		/// for equality. Returns true if the
		/// Strings equals method for the String
		/// representing the two portlet modes returns true.
		/// 
		/// Parameter mode is the portlet mode to compare this portlet mode with
		/// 
		/// Returns true, if the specified object is equal with this portlet mode

private: 
	PortletMode();

	std::string _name;
};


/// inlines


inline bool PortletMode::equals(const PortletMode& mode) 
{
	return _name == mode._name;
}


inline const std::string& PortletMode::toString() const
{
	return _name;
}


Portlet_END


#endif //Portlet_PortletMode_INCLUDED
