//
// Document.h
//
// $Id$
//
// Library: MongoDB
// Package: MongoDB
// Module:  Document
//
// Definition of the Document class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
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

#ifndef _MongoDB_Document_included
#define _MongoDB_Document_included

#include <algorithm>

#include "Poco/BinaryReader.h"
#include "Poco/BinaryWriter.h"

#include "Poco/MongoDB/MongoDB.h"
#include "Poco/MongoDB/Element.h"

namespace Poco
{
namespace MongoDB
{

class ElementFindByName
{
public:
	ElementFindByName(const std::string& name) : _name(name)
	{
	}

	bool operator()(const Element::Ptr& element)
	{
		return !element.isNull() && element->name() == _name;
	}

private:
	std::string _name;
};

class MongoDB_API Document
{
public:

	typedef SharedPtr<Document> Ptr;


	typedef std::vector<Document::Ptr> Vector;


	Document();


	virtual ~Document();

	
	void read(BinaryReader& reader);

	
	void write(BinaryWriter& writer);

	
	template<typename T>
	T get(const std::string& name)
	{
		Element::Ptr element = get(name);
		if ( element.isNull() )
		{
			throw Poco::NotFoundException(name);
		}
		else
		{
			if ( ElementTraits<T>::TypeId == element->type() )
			{
				ConcreteElement<T>* concrete = dynamic_cast<ConcreteElement<T>* >(element.get());
				return concrete->value();
			}
			else
			{
				throw std::runtime_error("Invalid type mismatch!");
			}
		}
	}

	Element::Ptr get(const std::string& name)
	{
		Element::Ptr element;

		ElementSet::iterator it = std::find_if(_elements.begin(), _elements.end(), ElementFindByName(name));
		if ( it != _elements.end() )
		{
			return *it;
		}

		return element;
	}

	bool exists(const std::string& name)
	{
		return std::find_if(_elements.begin(), _elements.end(), ElementFindByName(name)) != _elements.end();
	}


	template<typename T>
	bool isType(const std::string& name)
	{
		Element::Ptr element = get(name);
		if ( element.isNull() )
		{
			return false;
		}

		return ElementTraits<T>::TypeId == element->type();
	}


	void addElement(Element::Ptr element);


	template<typename T>
	void add(const std::string& name, T value)
	{
		addElement(new ConcreteElement<T>(name, value));
	}


	bool empty() const;


	void clear();


	std::string toString() const;


private:

	ElementSet _elements;
};


inline bool Document::empty() const
{
	return _elements.empty();
}


inline void Document::clear()
{
	_elements.clear();
}

// BSON Embedded Document
// spec: document
template<>
struct ElementTraits<Document::Ptr>
{
	enum { TypeId = 0x03 };

	static std::string toString(const Document::Ptr& value)
	{
		//TODO
		return value.isNull() ? "null" : "{}";
	}
};

template<>
inline void BSONReader::read<Document::Ptr>(Document::Ptr& to)
{
	to->read(_reader);
}

template<>
inline void BSONWriter::write<Document::Ptr>(Document::Ptr& from)
{
	from->write(_writer);
}

}} // Namespace Poco::MongoDB

#endif //  _MongoDB_Document_included