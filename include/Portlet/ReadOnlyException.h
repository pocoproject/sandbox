/// ReadonlyException.h

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_ReadOnlyException_INCLUDED
#define Portlet_ReadOnlyException_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletException_INCLUDED
#include "Portlet/PortletException.h"
#endif


Portlet_BEGIN


class CPP_PORTLET_DLL_API ReadOnlyException : public PortletException
	/// The ReadOnlyException is thrown when a portlet tries
	/// to change the value for a read-only preference attribute.
{
public:
	ReadOnlyException(const std::string text);
  		/// Constructs a new read-only exception with the given text. The
  		/// portlet container may use the text write it to a log.

	~ReadOnlyException() throw();

private:
	ReadOnlyException();
};


Portlet_END


#endif // Portlet_ReadOnlyException_INCLUDED