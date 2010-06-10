//
// Function.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Function
//
// Definition of Function.
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


#ifndef SpiderMonkey_Function_INCLUDED
#define SpiderMonkey_Function_INCLUDED


#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Arguments.h"
#include "Poco/DynamicAny.h"
#include "Poco/SharedPtr.h"

#include <vector>

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class Context;
class Arguments;

class SpiderMonkey_API Function : public Object
{
public:
  Function(const Object& obj);


  Function(const Poco::DynamicAny& any);


  Function(const Object& obj, const std::string &name);


  Function(const Function&);


  Function& operator = (const Function&);


  virtual ~Function();


  virtual bool isValid() const;


  void addArgument(const DynamicAny& arg);


  Arguments& arguments();


  int getIndex();


  Poco::DynamicAny& operator[](int arg);
  
  
  bool hasArguments() const;


  std::string name() const;


private:


  Arguments _arguments;


  friend class Object;
};

inline bool Function::hasArguments() const
{
  return _arguments.size() > 0;
}

}}}}


namespace Poco
{
template <>
class DynamicAnyHolderImpl<Poco::Script::JavaScript::SpiderMonkey::Function>: public DynamicAnyHolder
{
public:
	DynamicAnyHolderImpl(const Poco::Script::JavaScript::SpiderMonkey::Function& val): _val(val)
	{
	}

	~DynamicAnyHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(Poco::Script::JavaScript::SpiderMonkey::Function);
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

	void convert(std::string& s) const
	{
	    if ( *_val == JSVAL_NULL )
	    {
	      s.clear();
	      return;
	    }

	    JSString *str = JS_ValueToString(*_val.context(), OBJECT_TO_JSVAL(*_val));
	    if ( str != NULL )
	    {
	      jschar* jsstr = JS_GetStringChars(str);
	      size_t srclen = JS_GetStringLength(str);

	      if ( srclen > 0 )
	      {
	        size_t dstlen = 0;
	        if ( JS_EncodeCharacters(*_val.context(), jsstr, srclen, NULL, &dstlen) )
	        {
	          char *dst = (char *) JS_malloc(*_val.context(), dstlen);
	          if (dst != NULL)
	          {
	            JS_EncodeCharacters(*_val.context(), jsstr, srclen, dst, &dstlen);
	            s.assign(dst, dstlen);
	            JS_free(*_val.context(), dst);
	          }
	        }
	      }
	    }
	}

	void convert(DateTime& val) const
	{
		throw BadCastException();
	}

	void convert(LocalDateTime& ldt) const
	{
		throw BadCastException();
	}

	void convert(Timestamp& ts) const
	{
		throw BadCastException();
	}

	DynamicAnyHolder* clone() const
	{
		return new DynamicAnyHolderImpl(_val);
	}
	
	const Poco::Script::JavaScript::SpiderMonkey::Function& value() const
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
	Poco::Script::JavaScript::SpiderMonkey::Function _val;
};


} // namespace Poco


#endif
