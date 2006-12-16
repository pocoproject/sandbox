#include "Portlet/PortletMode.h"
#include "Poco/String.h"
#include "Poco/Exception.h"

using Poco::toLower;
using Poco::InvalidArgumentException;

Portlet_BEGIN


const PortletMode PortletMode::VIEW("view");
const PortletMode PortletMode::EDIT("edit");
const PortletMode PortletMode::HELP("help");


PortletMode::PortletMode(const std::string& name) 
{
    if ("" == name)
      throw new InvalidArgumentException("PortletMode name can not be empty");
 
    _name = toLower(name);
}


Portlet_END
