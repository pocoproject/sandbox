/// WindowStateException.cpp

#include "Portlet/WindowStateException.h"
#include "Portlet/WindowState.h"

Portlet_BEGIN


WindowStateException::WindowStateException(const std::string& text, const WindowState& state):
	  PortletException(text), _state(state)
{
}
  

WindowStateException::~WindowStateException() throw()
{
}


Portlet_END