/// PortletSessionUtil.cpp

#include "Portlet/PortletSessionUtil.h"
#include "Portlet/PortletSession.h"

Portlet_BEGIN

const std::string PortletSessionUtil::PORTLET_SCOPE_NAMESPACE = "javax.portlet.p.";

std::string& PortletSessionUtil::decodeAttributeName(std::string& name)
{
	if (0 == name.find(PORTLET_SCOPE_NAMESPACE)) 
	{
		int index = name.find_first_of('?');
		if (name.npos != index) 
		{
			name = name.substr(name.length() - index);
		}
	}

	return name;
}


int PortletSessionUtil::decodeScope(const std::string& name)
{
	int scope = PortletSession::APPLICATION_SCOPE; // APP
	if (0 == name.find(PORTLET_SCOPE_NAMESPACE)) 
	{
		int index = name.find_first_of('?');
		if (name.npos != index) 
		{
			scope = PortletSession::PORTLET_SCOPE; // PORTLET
		}
	}

	return scope;
}


Portlet_END

