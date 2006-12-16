/// UnavailableException.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_UnavailableException_INCLUDED
#define Portlet_UnavailableException_INCLUDED


#ifndef Portlet_PortletException_INCLUDED
#include "Portlet/PortletException.h"
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API UnavailableException : public PortletException 
	/// The portlet should throw the UnavailableException when 
	/// the portlet is either temporarily or permanently unavailable to handle requests.
{
public:
	UnavailableException(const std::string& msg);
	UnavailableException(const std::string& msg, int seconds);

	~UnavailableException() throw();

	bool isPermanent();
		/// Returns true if the exception is permanent.

	int getUnavailableSeconds();
		/// Returns number of seconds in the period for which the portlet is expected
		/// to be unavailable or -1 if the servlet is permanently unavailable.

private:
	bool _permanent;
	int _seconds;

	static const int PERMANENT_SECONDS;
};

///  inlines
inline bool UnavailableException::isPermanent() 
{
	return _permanent;
}

inline int UnavailableException::getUnavailableSeconds() 
{
	return _permanent ? PERMANENT_SECONDS : _seconds;
}

Portlet_END

#endif//Portlet_UnavailableException_INCLUDED
