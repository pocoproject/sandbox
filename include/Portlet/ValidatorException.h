/// ValidatorException.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.


#ifndef Portlet_ValidatorException_INCLUDED
#define Portlet_ValidatorException_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
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


class CPP_PORTLET_DLL_API ValidatorException : public PortletException
	/// The ValidatorException is thrown by the
	/// validate method of a PreferencesValidator when 
	/// the validation of a preference failed.
{
public:
	ValidatorException (const std::string& text, const std::vector<std::string>& failedKeys);
  		/// Constructs a new portlet validator exception when the portlet needs to throw an
  		/// exception. The exception message is based on the localized message
  		/// of the underlying exception.
  		/// 
  		/// The vector of failed keys may contain all failed keys, only the
  		/// first key that failed validation, or may be empty.
  		///
  		/// Parameter text is the exception text.
  		/// Parameter failedKeys keys that failed the validation.

	~ValidatorException() throw();

	const std::vector<std::string>& getFailedKeys() const;
  		/// Returns the keys that failed the validation.
  		/// 
  		/// The string vector of failed keys may contain all failed keys, only the
  		/// first key that failed validation, or an empty 
  		/// string vector if no failed keys are available.

private: 
	ValidatorException();
	std::vector<std::string> _failedKeyVector;
};


/// inlines


inline const std::vector<std::string>& ValidatorException::getFailedKeys() const
{
  return _failedKeyVector;
}


Portlet_END


#endif // Portlet_ValidatorException_INCLUDED
