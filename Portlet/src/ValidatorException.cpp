/// ValidatorException.cpp

#include "Portlet/ValidatorException.h"
#include "Portlet/PortletException.h"


Portlet_BEGIN


ValidatorException::ValidatorException(const std::string& text, const std::vector<std::string>& failedKeys):
  PortletException(text)
{
  _failedKeyVector.assign(failedKeys.begin(), failedKeys.end());
}


ValidatorException::~ValidatorException() throw()
{
}

Portlet_END;