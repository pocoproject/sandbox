/// GenericPortlet.cpp

#include "Portlet/GenericPortlet.h"


Portlet_BEGIN


GenericPortlet::GenericPortlet() : _pConfig(0)
{
}


GenericPortlet::~GenericPortlet()
{
	delete _pConfig;
}


void GenericPortlet::init(PortletConfig* pConfig)
{
	_pConfig = pConfig;
	init();
}


void GenericPortlet::init()
{
	checkConfig();
}


void GenericPortlet::destroy()
{
}



void GenericPortlet::processAction(const ActionRequest& request, ActionResponse& response)
{
  throw PortletException("processAction method not implemented");
}


void GenericPortlet::render(const RenderRequest& request, RenderResponse& response)
{
  response.setTitle(getTitle(request));
  doDispatch(request, response);
}


std::string GenericPortlet::getTitle(const RenderRequest& request) const
{
	return "";//config.getResourceBundle(request.getLocale()).getstring("javax.portlet.title");
}


void GenericPortlet::doDispatch(const RenderRequest& request, RenderResponse& response)
{
	WindowState state = request.getWindowState();

	if (!state.equals(WindowState::MINIMIZED)) 
	{
		PortletMode mode = request.getPortletMode();
		if (mode.equals(PortletMode::VIEW)) doView(request, response);
		else if (mode.equals(PortletMode::EDIT)) doEdit(request, response);
		else if (mode.equals(PortletMode::HELP)) doHelp(request, response);
		else throw PortletException("unknown portlet mode: " + mode.toString());
	}
}


void GenericPortlet::doView(const RenderRequest& request, RenderResponse& response)
{
  throw PortletException("doView method not implemented");
}


void GenericPortlet::doEdit(const RenderRequest& request, RenderResponse& response)
{
  throw PortletException("doEdit method not implemented");
}


void GenericPortlet::doHelp(const RenderRequest& request, RenderResponse& response)
{
  throw PortletException("doHelp method not implemented");

}


const PortletConfig& GenericPortlet::getPortletConfig()
{
	checkConfig();
	return *_pConfig;
}


// PortletConfig implementation

const std::string& GenericPortlet::getPortletName()
{
	checkConfig();
	return _pConfig->getPortletName();
}


const PortletContext& GenericPortlet::getPortletContext()
{
	checkConfig();
	return _pConfig->getPortletContext();
}


const std::string& GenericPortlet::getInitParameter(const std::string& name)
{
	checkConfig();
	return _pConfig->getInitParameter(name);
}


const std::vector<std::string>& GenericPortlet::getInitParameterNames()
{
	checkConfig();
	return _pConfig->getInitParameterNames();
}


Portlet_END
