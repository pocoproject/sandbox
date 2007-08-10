//
// ListView.cpp
//
// $Id: //poco/Main/WebWidgets/src/ListView.cpp#3 $
//
// Library: WebWidgets
// Package: WebGUI
// Module:  ListView
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#include "Poco/WebWidgets/ListView.h"
#include "Poco/WebWidgets/XMLUtil.h"
#include "Poco/WebWidgets/Visitor.h"
#include "Poco/DOM/Element.h"
#include "Poco/StringTokenizer.h"


namespace Poco {
namespace WebWidgets {


const std::string ListView::FACTORY_ID("listView");
const std::string ListView::XML_MULTIPLE("multiple");
const std::string ListView::XML_SIZE("size");


ListView::ListView(const std::string& uniqueName, int viewSize, bool multiple, const std::vector<Poco::AutoPtr<Label> >& children):
	ComboBox(uniqueName, children),
	_selected(),
	_multiple(multiple),
	_size(viewSize)
{
}


ListView::ListView(const std::string& uniqueName, int viewSize, bool multiple, const std::vector<Poco::AutoPtr<Widget> >& children):
	ComboBox(uniqueName, children),
	_selected(),
	_multiple(multiple),
	_size(viewSize)
{
}


ListView::ListView(const Poco::XML::Element* pElem):
	ComboBox(pElem),
	_selected(),
	_multiple(XMLUtil::getBoolAttr(pElem, XML_MULTIPLE, false)),
	_size(XMLUtil::getNumericAttr<int>(pElem, XML_SIZE))
{
	const std::string& attr = pElem->getAttribute(XML_SELECT);
	Poco::StringTokenizer tok(attr, ", ", Poco::StringTokenizer::TOK_IGNORE_EMPTY | Poco::StringTokenizer::TOK_TRIM);
	Poco::StringTokenizer::Iterator it = tok.begin();
	Poco::StringTokenizer::Iterator itEnd = tok.end();
	for (; it != itEnd; ++it)
	{
		int tmp = Poco::NumberParser::parse(*it);
		select(tmp);
	}
}


ListView::~ListView()
{
}


int ListView::getSelectedIndex() const
{
	if (_selected.empty())
		return -1;
	return *_selected.begin();
}


void ListView::select(int idx)
{
	if (idx >= 0)
		_selected.insert(idx);
}


void ListView::accept(Visitor& v)
{
	v.visit(*this);
}


Poco::AutoPtr<Widget> ListView::clone() const
{
	Poco::AutoPtr<ListView> ptr = new ListView(getName(), _size, _multiple, cloneChildren());
	ptr->setAttributes(getAttributes());
	const std::set<int>& sels = getSelectedIndizes();
	std::set<int>::const_iterator it = sels.begin();
	std::set<int>::const_iterator itEnd = sels.end();
	for(; it != itEnd; ++it)
		ptr->select(*it);

	return ptr;
}

} } // namespace Poco::WebWidgets
