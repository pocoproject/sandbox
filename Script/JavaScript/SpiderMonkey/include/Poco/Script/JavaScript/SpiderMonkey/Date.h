//
// Date.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Date
//
// Definition of Date.
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


#ifndef SpiderMonkey_Date_INCLUDED
#define SpiderMonkey_Date_INCLUDED

#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/SharedPtr.h"
#include "Poco/Any.h"

#include <jsapi.h>
#include <jsdate.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class SpiderMonkey_API Date : public Object
	/// SpiderMonkey Date class. Encapsulates Date class.
{
public:
  Date(const Object& obj);
    /// Constructs a Date class from an Object


  Date(const Context& context, JSObject* obj);
    /// Constructs a Date class from a SpiderMonkey object


  Date(const Poco::DynamicAny& any);
    /// Constructs a Date class from DynamicAny


	Date(const Date&);
    /// Copy constructor


	Date& operator = (const Date&);


	virtual ~Date();
    /// Destructor


  virtual bool isValid() const;
    /// Returns true when this is a valid Date object


  static Date newDate(const Context& cx, const Poco::DateTime &date);
    /// Creates a new Date object

  
  int getYear() const;
    /// Returns the year


  int getMonth() const;
    /// Returns the month


  int getDay() const;
    /// Returns the day


  int getHours() const;
    /// Returns the hours


  int getMinutes() const;
    /// Returns the minutes


  int getSeconds() const;
    /// Returns the seconds


private:


	friend class Context;
};

inline int Date::getYear() const
{
  return js_DateGetYear(*context(), _obj);
}

inline int Date::getMonth() const
{
  return js_DateGetMonth(*context(), _obj);
}

inline int Date::getDay() const
{
  return js_DateGetDate(*context(), _obj);
}

inline int Date::getHours() const
{
  return js_DateGetHours(*context(), _obj);
}

inline int Date::getMinutes() const
{
  return js_DateGetMinutes(*context(), _obj);
}

inline int Date::getSeconds() const
{
  return js_DateGetSeconds(*context(), _obj);
}

} } } } // namespace Poco::Script::JavaScript::SpiderMonkey


namespace Poco
{
template <>
class DynamicAnyHolderImpl<Script::JavaScript::SpiderMonkey::Date>: public DynamicAnyHolder
{
public:
	DynamicAnyHolderImpl(const Script::JavaScript::SpiderMonkey::Date& val): _val(val)
	{
	}

	~DynamicAnyHolderImpl()
	{
	}
	
	const std::type_info& type() const
	{
		return typeid(Script::JavaScript::SpiderMonkey::Date);
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
    if ( _val.isValid() )
    {
      val.assign(_val.getYear(),
                 _val.getMonth() + 1,
                 _val.getDay(),
                 _val.getHours(),
                 _val.getMinutes(),
                 _val.getSeconds());
    }
	}

	void convert(LocalDateTime& ldt) const
	{
	}

	void convert(Timestamp& ts) const
	{
	}

	DynamicAnyHolder* clone() const
	{
		return new DynamicAnyHolderImpl(_val);
	}
	
	const Script::JavaScript::SpiderMonkey::Date& value() const
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
	Script::JavaScript::SpiderMonkey::Date _val;
};


}; // namespace Poco

#endif // SpiderMonkey_Date_INCLUDED
