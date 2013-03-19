//
// SOAPEnvelope.cpp
//
// $Id: //poco/1.1.0/XML/src/SOAPEnvelope.cpp#1 $
//
// Library: Web
// Package: SOAP
// Module:  SOAPEnvelope
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
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


#include "Poco/Web/SOAPEnvelope.h"
#include "Poco/Web/SOAPMessage.h"
#include "Poco/Web/SOAPHeader.h"
#include "Poco/DOM/Document.h"
#include "Poco/DOM/Node.h"
#include "Poco/DOM/Element.h"
#include "Poco/DOM/NodeList.h"
#include "Poco/Exception.h"


using Poco::InvalidAccessException;
using Poco::XML::Document;
using Poco::XML::Node;
using Poco::XML::Element;
using Poco::XML::Text;
using Poco::XML::NodeList;


namespace Poco {
namespace Web {


SOAPEnvelope::SOAPEnvelope(Document* pDoc, SOAPElement::Version version, bool header):
	SOAPElement(pDoc, version),
	_pHeader(0),
	_pBody(0)
{
	_pEnvelope = document().createElement(envelopeTag());
	Element* pEnv = _pEnvelope.get();
	poco_check_ptr(pEnv);
	pEnv->setAttribute("xmlns:SOAP-ENV", "http://schemas.xmlsoap.org/soap/envelope/");
	pEnv->setAttribute("xmlns:xsd", "http://www.w3.org/2001/XMLSchema");
	pEnv->setAttribute("xmlns:xsi", "http://www.w3.org/2001/XMLSchema-instance");
	pEnv->setAttribute("xmlns:SE", "http://schemas.xmlsoap.org/soap/encoding/");
	document().appendChild(pEnv);

	if (header) _pHeader =  new SOAPHeader(&document());
	_pBody = new SOAPBody(&document());
}


SOAPEnvelope::~SOAPEnvelope()
{
	delete _pHeader;
}


void SOAPEnvelope::add(const std::string& tag, const std::string& content)
{
	SOAPElement::add(_pEnvelope.get(), tag, content);
}


NodeList* SOAPEnvelope::get(const std::string& tag) const
{
	if (_pEnvelope) _pEnvelope->getElementsByTagName(tag);

	return 0;
}


void SOAPEnvelope::addHeaderElement(const std::string& tag, const std::string& content)
{
	poco_check_ptr(_pHeader);
	_pHeader->add(tag, content);
}



NodeList* SOAPEnvelope::getHeaderElements(const std::string& name) const
{
	if (_pHeader) return _pHeader->get(name);

	return 0;
}


void SOAPEnvelope::addBodyElement(const std::string& tag, const std::string& content)
{
	poco_check_ptr(_pBody);
	_pBody->add(tag, content);
}



NodeList* SOAPEnvelope::getBodyElements(const std::string& name) const
{
	poco_check_ptr(_pBody);
	return _pBody->get(name);
}


} } // namespace Poco::Web 