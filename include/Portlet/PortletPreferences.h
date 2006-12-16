/// PortletPreferences.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_PortletPreferences_INCLUDED
#define Portlet_PortletPreferences_INCLUDED


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
#ifndef STD_MAP_INCLUDED
#include <map>
# define STD_MAP_INCLUDED
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API PortletPreferences
	/// The PortletPreferences interface allows the portlet to store
	/// configuration data. It is not the
	/// purpose of this interface to replace general purpose databases.
	/// 
	/// There are two different types of preferences:
	/// 
	/// modifiable preferences - these preferences can be changed by the
	///     portlet in any standard portlet mode (EDIT, HELP, VIEW).
	///     Per default every preference is modifiable.
	/// read-only preferences - these preferences cannot be changed by the
	///     portlet in any standard portlet mode, but may be changed by administrative modes.
	///     Preferences are read-only, if the are defined in the
	///     deployment descriptor with read-only set to true,
	///     or if the portlet container restricts write access.
	/// 
	/// 
	/// Changes are persisted when the store method is called. The store method 
	/// can only be invoked within the scope of a processAction call.
	/// Changes that are not persisted are discarded when the
	/// processAction or render method ends.
{
public:
	virtual bool isReadOnly(const std::string& key) const = 0;
  		/// Returns true, if the value of this key cannot be modified by the user.
  		/// 
  		/// Modifiable preferences can be changed by the
  		/// portlet in any standard portlet mode (EDIT, HELP, VIEW). 
  		/// Per default every preference is modifiable.
  		/// 
  		/// Read-only preferences cannot be changed by the
  		/// portlet in any standard portlet mode, but inside of custom modes
  		/// it may be allowed changing them.
  		/// Preferences are read-only, if they are defined in the 
  		/// deployment descriptor with read-only set to true,
  		/// or if the portlet container restricts write access.
  		///
  		/// Returns false, if the value of this key can be changed, or
  		///          if the key is not known
  		///
  		/// Throws IllegalArgumentException if key is null.
  
	virtual const std::string& getValue(const std::string& key, const std::string& def) const = 0;
  		/// Returns the first String value associated with the specified key of this preference.
  		/// If there is one or more preference values associated with the given key 
  		/// it returns the first associated value.
  		/// If there are no preference values associated with the given key, or the 
  		/// backing preference database is unavailable, it returns the given 
  		/// default value.
  		///
  		/// Parameter key is the key for which the associated value is to be returned
  		/// Parameter def is the value to be returned in the event that there is no 
  		///            value available associated with this key.
  		///
  		/// Returns the value associated with key, or def
  		///         if no value is associated with key, or the backing
  		///         store is inaccessible.
  		///
  		/// Throws IllegalArgumentException if key is null. 
		///		(An empty string for def is permitted.)

	virtual const std::vector<std::string>& getValues(const std::string& key, 
		const std::vector<std::string>& def) const = 0;
  		/// Returns the String array value associated with the specified key in this preference.
  		///
  		/// Returns the specified default if there is no value
  		/// associated with the key, or if the backing store is inaccessible.
  		///
  		/// If the implementation supports stored defaults and such a
  		/// default exists and is accessible, it is used in favor of the
  		/// specified default.
  		///
  		///
  		/// Parameter key is the key for which associated value is to be returned.
  		/// Parameter def is the value to be returned in the event that this
  		///        preference node has no value associated with key
  		///        or the associated value cannot be interpreted as a String array,
  		///        or the backing store is inaccessible.
  		///
  		/// Returns the string vector value associated with
  		///         key, or def if the associated value does not exist.
  		///
  		/// Throws IllegalArgumentException if key is null.   
  		///         (An empty string value for def is permitted.)
  
	virtual void setValue(const std::string& key, const std::string& value) = 0;
  		/// Associates the specified String value with the specified key in this
  		/// preference.
  		/// 
  		/// The key cannot be null, but null values
  		/// for the value parameter are allowed.
  		///
  		/// Parameter key is the key with which the specified value is to be associated.
  		/// Parameter value is te value to be associated with the specified key.
  		///
  		/// Throws ReadOnlyException
  		///                 if this preference cannot be modified for this request
  		/// Throws IllegalArgumentException if key is null,
  		///                 or key.length() 
  		///                 or value.length are to long. The maximum length 
  		///                 for key and value are implementation specific.

	virtual void setValues(const std::string& key, const std::vector<std::string>& values) = 0;
  		/// Associates the specified String array value with the specified key in this
  		/// preference.
  		/// 
  		/// The key cannot be null, but null values
  		/// in the values parameter are allowed.
  		///
  		/// @param key key with which the  value is to be associated
  		/// @param values values to be associated with key
  		///
  		/// Throws IllegalArgumentException if key is empty string, or
  		///                 key.length() is to long or value.size is to large.
  		/// Throws  ReadOnlyException
  		///                 if this preference cannot be modified for this request
 
	virtual const std::vector<std::string>& getNames() const = 0;
  		/// Returns all of the keys that have an associated value,
  		/// or an empty vector if no keys are
  		/// available.
  		///
  		/// Returns a vector of the keys that have an associated value,
  		/// or an empty Enumeration if no keys are available.

	virtual const std::map<std::string, std::vector<std::string> >& getMap() const = 0;
  		/// Returns a map of the preferences.
  		/// 
  		/// The values in the returned map are of type
  		/// std::vector<std::string>.
  		/// 
  		/// If no preferences exist this method returns an empty Map.
  		///
  		/// Returns an immutable Map containing preference names as 
  		/// keys and preference values as map values, or an empty Map
  		/// if no preference exist. The keys in the preference
  		/// map are of type String. The values in the preference map are of type
		/// std::map<std::string, std::vector<std::string> >.

	virtual void reset(const std::string& key) = 0;
  		/// Resets or removes the value associated with the specified key.
  		/// 
  		/// If this implementation supports stored defaults, and there is such
  		/// a default for the specified preference, the given key will be 
  		/// reset to the stored default.
  		/// 
  		/// If there is no default available the key will be removed.
  		///
  		/// Parameter is key to reset
  		///
  		/// Throws IllegalArgumentException if key is null.
  		/// Throws ReadOnlyException if this preference cannot be modified 
		/// for this request

	virtual void store() = 0;
		/// Commits all changes made to the preferences via the 
  		/// set methods in the persistent store.
  		/// 
  		/// If this call returns succesfull, all changes are made
  		/// persistent. If this call fails, no changes are made
  		/// in the persistent store. This call is an atomic operation 
  		/// regardless of how many preference attributes have been modified.
  		/// 
  		/// All changes made to preferences not followed by a call 
  		/// to the store method are discarded when the 
  		/// portlet finishes the processAction method.
  		/// 
  		/// If a validator is defined for this preferences in the
  		/// deployment descriptor, this validator is called before
  		/// the actual store is performed to check wether the given
  		/// preferences are vaild. If this check fails a 
  		/// ValidatorException is thrown.
  		///
		/// Throws :
		///		IOException    
  		///                 if changes cannot be written into
  		///                 the backend store
  		///		ValidatorException
  		///                 if the validation performed by the
  		///                 associated validator fails
  		///		IllegalStateException
  		///                 if this method is called inside a render call

};


Portlet_END


#endif //Portlet_PortletPreferences_INCLUDED
