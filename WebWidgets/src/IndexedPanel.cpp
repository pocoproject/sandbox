//
// IndexedPanel.cpp
//
// $Id: //poco/Main/WebWidgets/src/IndexedPanel.cpp#5 $
//
// Library: WebWidgets
// Package: WebGUI
// Module:  IndexedPanel
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


#include "Poco/WebWidgets/IndexedPanel.h"
#include "Poco/WebWidgets/Visitor.h"
#include "Poco/WebWidgets/XMLUtil.h"


namespace Poco {
namespace WebWidgets {


IndexedPanel::IndexedPanel(const std::vector<Poco::AutoPtr<Widget> >& children):
	ComplexPanel(),
	_children(children)
{
	std::vector<Poco::AutoPtr<Widget> >::iterator it = _children.begin();
	std::vector<Poco::AutoPtr<Widget> >::iterator itEnd = _children.end();
	for (; it != itEnd; ++it)
		(*it)->setParent(this);
}


IndexedPanel::IndexedPanel(const Poco::XML::Element* pElem):
	ComplexPanel(pElem),
	_children()
{
	_children = XMLUtil::getAllWidgets(pElem, this);
}


IndexedPanel::IndexedPanel(const Poco::XML::Element* pElem, const std::string& filter):
	ComplexPanel(pElem),
	_children()
{
	_children = XMLUtil::getWidgets(pElem, filter, this);
}


IndexedPanel::~IndexedPanel()
{
}


void IndexedPanel::accept(Visitor& v)
{
	v.visit(*this);
}



void IndexedPanel::preAccept(Visitor& v)
{
	v.preVisit(*this);
}


Poco::AutoPtr<Widget> IndexedPanel::find(const std::string& id) const
{
	std::vector<Poco::AutoPtr<Widget> >::const_iterator it = _children.begin();
	std::vector<Poco::AutoPtr<Widget> >::const_iterator itEnd = _children.end();
	Poco::AutoPtr<Widget> res;
	for (; it != itEnd && res.isNull(); ++it)
	{
		res = Widget::find((*it), id);
	}

	return res;
}


Poco::AutoPtr<Widget> IndexedPanel::clone() const
{
	Poco::AutoPtr<Widget> ptr = new IndexedPanel(cloneChildren());
	ptr->setAttributes(getAttributes());
	return ptr;
}


std::vector<Poco::AutoPtr<Widget> > IndexedPanel::cloneChildren() const
{
	std::vector<Poco::AutoPtr<Widget> > result;
	std::vector<Poco::AutoPtr<Widget> >::const_iterator it = _children.begin();
	std::vector<Poco::AutoPtr<Widget> >::const_iterator itEnd = _children.end();
	for (; it != itEnd; ++it)
	{
		if ((*it))
			result.push_back((*it)->clone());
		else
			result.push_back(Poco::AutoPtr<Widget>());
	}
	return result;
}


} } // namespace Poco::WebWidgets
