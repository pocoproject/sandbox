/// WindowState.cpp

#include "Portlet/WindowState.h"
#include "Poco/String.h"
#include "Poco/Exception.h"

using Poco::toLower;
using Poco::InvalidArgumentException;

Portlet_BEGIN

  
const WindowState WindowState::NORMAL("normal");
const WindowState WindowState::MAXIMIZED("maximized");
const WindowState WindowState::MINIMIZED("minimized");


WindowState::WindowState(const std::string& name) 
{
  if ("" == name)
    throw InvalidArgumentException("WindowState name can not be empty");

  _name = toLower(name);
}


Portlet_END