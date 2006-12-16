/// WindowState.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.



#ifndef Portlet_WindowState_INCLUDED
#define Portlet_WindowState_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef STD_STRING_INCLUDED
#include <string>
# define STD_STRING_INCLUDED
#endif

Portlet_BEGIN


class CPP_PORTLET_DLL_API WindowState
	/// The WindowState class represents
	/// the possible window states that a portlet window can assume.
	/// 
	/// This class defines a standard set of the most basic portlet window states.
	/// Additional window states may be defined by calling the constructor of
	/// this class. If a portal/portlet-container does not support a 
	/// custom window state defined in the portlet application deployment descriptor, 
	/// the custom window state will be ignored by the portal/portlet container.
{
public:  
	static const WindowState NORMAL;
		/// The NORMALwindow state indicates that a portlet 
		/// may be sharing the page with other portlets. It may also 
		/// indicate that the target device has limited display capabilities.
		/// Therefore, a portlet should restrict the size of its rendered 
		/// output in this window state.
		/// 
		/// The string value for this state is "normal".
  
	static const WindowState MAXIMIZED;
		/// The MAXIMIZEDwindow state is an indication 
		/// that a portlet may be the only portlet being rendered in the 
		/// portal page, or that the portlet has more space compared to other portlets
		/// in the portal page. A portlet may generate richer content 
		/// when its window state is MAXIMIZED.
		/// 
		/// The string value for this state is "maximized".
 
	static const WindowState MINIMIZED;
		/// When a portlet is in MINIMIZEDwindow state, 
		/// the portlet should only render minimal output or no output at all.
		/// 
		/// The string value for this state is "minimized".

	WindowState(const std::string& name);
		/// Creates a new window state with the given name.
		/// 
		/// Upper case letters in the name are converted to
		/// lower case letters.
		///
		/// Parameter name is the name of the portlet mode
  
	std::string toString();
		/// Returns a String representation of this window state.
		/// Window state names are always lower case names.

		
	bool equals(const WindowState& state);
		/// Compares the specified object with this window state
		/// for equality. Returns trueif the
		/// Strings equalsmethod for the String
		/// representing the two window states returns true.
		/// 
		/// @Parameter state is the window state to compare this window state with.
		/// 
		/// Returns true, if the specified object is equal with this window state.
  

private: 
	WindowState();

	std::string _name;
};


/// inlines


inline bool WindowState::equals(const WindowState& state) 
{
    return _name == state._name;
}


inline std::string WindowState::toString() 
{
    return _name;
}


Portlet_END


#endif //Portlet_WindowState_INCLUDED
