//
// Array.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Array
//
// Definition of Array.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions
// are met:
//
// 1. Redistributions of source code must retain the above copyright
//    notice, this list of conditions and the following disclaimer.
//
// 2. Redistributions in binary form must reproduce the above copyright
//    notice, this list of conditions and the following disclaimer in the
//    documentation and/or other materials provided with the distribution.
//
// 3. Redistributions in any form must be accompanied by information on
//    how to obtain complete source code for this software and any
//    accompanying software that uses this software.  The source code
//    must either be included in the distribution or be available for no
//    more than the cost of distribution plus a nominal fee, and must be
//    freely redistributable under reasonable conditions.  For an
//    executable file, complete source code means the source code for all
//    modules it contains.  It does not include source code for modules or
//    files that typically accompany the major components of the operating
//    system on which the executable file runs.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
// FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
// COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
// INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
// BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
// LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
// LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
// ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.
//


#ifndef SpiderMonkey_Array_INCLUDED
#define SpiderMonkey_Array_INCLUDED

#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/SharedPtr.h"
#include "Poco/Any.h"

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class SpiderMonkey_API Array : public Object
	/// Encapsulates Array class.
{
public:
  Array(const Object& obj);
    /// Creates an Array class from an Object

  Array(const Context& context, JSObject* obj);
    /// Creates an Array class from a SpiderMonkey Object


  Array(const Poco::DynamicAny& any);
    /// Creates an Array class from a value.


	Array(const Array&);
    /// Copy constructor


	Array& operator = (const Array&);
  
	virtual ~Array();
    /// Destructor


  void set(int index, const DynamicAny& Value);
    /// Set the value at the given index


  DynamicAny get(int index = 0) const;
    /// Return the value at the given index. An empty (void) value
    /// will be returned when the index is out of bound.


  int size() const;
    /// Returns the size of the array
    


  virtual bool isValid() const;
    /// Returns true when the object is a JavaScript Array object.


  DynamicAny operator[](int index) const;
    /// Returns trhe value at the given index.


  static Array newArray(const Context& cx, int elementCount = 0);
    /// Creates a new JavaScript Array object


private:


	friend class Context;
};


inline int Array::size() const
{
  if ( isValid() )
  {
    jsuint length;
    JS_GetArrayLength(*_context, _obj, &length);
    return length;
  }
  return 0;
}


} } } } // namespace Poco::Script::JavaScript::SpiderMonkey


namespace Poco
{
template <>
class DynamicAnyHolderImpl<Script::JavaScript::SpiderMonkey::Array>: public DynamicAnyHolder
{
public:
	DynamicAnyHolderImpl(const Script::JavaScript::SpiderMonkey::Array& val): _val(val)
	{
	}

	~DynamicAnyHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(Script::JavaScript::SpiderMonkey::Array);
	}

	void convert(Int8&) const
	{
		throw BadCastException();
	}

	void convert(Int16&) const
	{
		throw BadCastException();
	}
	
	void convert(Int32&) const
	{
		throw BadCastException();
	}

	void convert(Int64&) const
	{
		throw BadCastException();
	}

	void convert(UInt8&) const
	{
		throw BadCastException();
	}

	void convert(UInt16&) const
	{
		throw BadCastException();
	}
	
	void convert(UInt32&) const
	{
		throw BadCastException();
	}

	void convert(UInt64&) const
	{
		throw BadCastException();
	}

	void convert(bool&) const
	{
		throw BadCastException();
	}

	void convert(float&) const
	{
		throw BadCastException();
	}

	void convert(double&) const
	{
		throw BadCastException();
	}

	void convert(char&) const
	{
		throw BadCastException();
	}

	void convert(std::string& val) const
	{
//    Script::JavaScript::SpiderMonkey::Value v(_val);
//    val = v.toString();
	}

	void convert(DateTime& val) const
	{
		//TODO: val = _val;
	}

	void convert(LocalDateTime& ldt) const
	{
		//TODO: ldt = _val.timestamp();
	}

	void convert(Timestamp& ts) const
	{
		//TODO: ts = _val.timestamp();
	}

	DynamicAnyHolder* clone() const
	{
		return new DynamicAnyHolderImpl(_val);
	}
	
	const Script::JavaScript::SpiderMonkey::Array& value() const
	{
		return _val;
	}

	bool isArray() const
	{
		return false;
	}

	bool isInteger() const
	{
		return false;
	}

	bool isSigned() const
	{
		return false;
	}

	bool isNumeric() const
	{
		return false;
	}

	bool isString() const
	{
		return false;
	}

private:
	Script::JavaScript::SpiderMonkey::Array _val;
};


}; // namespace Poco

#endif
