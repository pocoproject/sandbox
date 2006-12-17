//
// GenericServlet.cpp
//
//
// Library: Servlet
// Package: Servlet
// Module:  GenericServlet
//
// Copyright (c) 2006, Aleksandar Fabijanic and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
// 
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//
// The code in this file is derived from Apache Tomcat project
// Copyright (c) 2004 The Apache Software Foundation.
// http://www.apache.org/licenses/LICENSE-2.0
// 


#include "Poco/Servlet/GenericServlet.h"


namespace Poco {
namespace Servlet {


GenericServlet::GenericServlet() : _pConfig(0)
{
}


GenericServlet::~GenericServlet() 
{
	delete _pConfig;
}


void GenericServlet::destroy()
{
	checkConfig();

	log("destroy : " + getServletName());

	delete _pConfig;
	_pConfig = 0;
}


const std::string& GenericServlet::getInitParameter(const std::string& name) const
{
	checkConfig();

	return _pConfig->getInitParameter(name);
}


std::vector<std::string> GenericServlet::getInitParameterNames() const
{
	checkConfig();

	return _pConfig->getInitParameterNames();
}


const ServletConfig& GenericServlet::getServletConfig() const
{
	checkConfig();

	return *_pConfig;
}


const ServletContext& GenericServlet::getServletContext() const
{
	checkConfig();

	return _pConfig->getServletContext();
}


std::string GenericServlet::getServletInfo() const
{
	return "";
}


const std::string& GenericServlet::getServletName() const
{
	checkConfig();

	return _pConfig->getServletName();
}


void GenericServlet::init()
{
}


void GenericServlet::init(ServletConfig* pConfig)
{
	_pConfig = pConfig;

	checkConfig();
	
	init();
}


void GenericServlet::log(const std::string& msg) const
{
	checkConfig();

	const ServletContext& sc = _pConfig->getServletContext();
	sc.log(msg);
}


void GenericServlet::log(const std::string& msg, const ServletException& e)
{
	checkConfig();

	_pConfig->getServletContext().log(msg, e);
}


} } // namespace Poco::Servlet