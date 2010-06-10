//
// Binder.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Binder
//
// Implementation of Binder
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
// All rights reserved.
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

#include "Poco/Script/ScriptException.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Binder.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Function.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Array.h"

#include <Poco/LocalDateTime.h>

#include <jsdate.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

Binder::Binder(const Context& context, jsval* value) : _context(context), _value(value)
{
}


Binder::~Binder()
{
}

    
void Binder::bind(const Poco::Int16& val, int pos)
{
  _value[pos] = INT_TO_JSVAL(val);
}
   
		
void Binder::bind(const Poco::UInt16& val, int pos)
{
  _value[pos] = INT_TO_JSVAL(val);
}
    
		
void Binder::bind(const Poco::Int32& val, int pos)
{
	if ( INT_FITS_IN_JSVAL(val) )
  {
    _value[pos] = INT_TO_JSVAL(val);
  }
  else
  {
    if ( ! JS_NewNumberValue(*_context, val, &_value[pos]) )
    {
      _value[pos] = JSVAL_NULL;
    }
  }
}
      
    
		
void Binder::bind(const Poco::UInt32& val, int pos)
{
	if ( INT_FITS_IN_JSVAL(val) )
  {
    _value[pos] = INT_TO_JSVAL(val);
  }
  else
  {
    if ( ! JS_NewNumberValue(*_context, val, &_value[pos]) )
    {
      _value[pos] = JSVAL_NULL;
    }
  }
}

void Binder::bind(const bool& val, int pos)
{
  _value[pos] = BOOLEAN_TO_JSVAL(val);
}    


void Binder::bind(const float& val, int pos)
{
  if ( ! JS_NewNumberValue(*_context, val, &_value[pos]) )
  {
    _value[pos] = JSVAL_NULL;
  }
}
    

void Binder::bind(const double& val, int pos)
{
  if ( ! JS_NewNumberValue(*_context, val, &_value[pos]) )
  {
    _value[pos] = JSVAL_NULL;
  }
}
    
		
void Binder::bind(const char& val, int pos)
{
  std::string s;
  s.push_back(val);
  bind(s, pos);
}

void Binder::bind(const std::string& val, int pos)
{
  if ( val.empty() )
  {
    _value[pos] = STRING_TO_JSVAL(JS_NewStringCopyN(*_context, "", 0));
  }
  else
  {
    _value[pos] = STRING_TO_JSVAL(JS_NewStringCopyN(*_context, val.c_str(), val.length()));
  }
}

void Binder::bind(const Function& val, int pos)
{
  JSObject* o = *val;
  _value[pos] = OBJECT_TO_JSVAL(o);
}

void Binder::bind(const Object& val, int pos)
{
  JSObject* o = *val;
  _value[pos] = OBJECT_TO_JSVAL(o);
}


void Binder::bind(const Timestamp& stamp, int pos)
{
  DateTime date(stamp);
  bind(date, pos);
}

void Binder::bind(const DateTime& date, int pos)
{
  Poco::LocalDateTime ldt(date);
  JSObject* o = js_NewDateObject(*_context, ldt.year(), ldt.month() - 1, ldt.day(), ldt.hour(), ldt.minute(), ldt.second());
  _value[pos] = OBJECT_TO_JSVAL(o);
}


void Binder::bind(const std::vector<Poco::DynamicAny> array, int pos)
{
  Array jsArray = Array::newArray(_context, array.size());
  for(std::vector<Poco::DynamicAny>::const_iterator it = array.begin(); it != array.end(); it++)
  {
    jsArray.set(it - array.begin(), *it);
  }
  bind(jsArray, pos);
}


void Binder::any(const Poco::DynamicAny& val, int pos)
{
  if ( val.isInteger() )
  {
    int i = val;
    bind(i, pos);
  }
  else if ( val.isNumeric() )
  {
    double d = val;
    bind(d, pos);
  }
  else if ( val.isString() )
  {
    std::string s = val;
    bind(s, pos);
  }
  else if ( val.isArray() )
  {
    std::vector<Poco::DynamicAny> arr = val.extract<std::vector<Poco::DynamicAny> >();
    bind(arr, pos);
  }
  else if ( val.isEmpty() )
  {
    _value[pos] = JSVAL_NULL;
  }
  else if ( val.type() == typeid(Function) )
  {
    Function f = val.extract<Function>();
    bind(f, pos);
  }
  else if ( val.type() == typeid(Object) )
  {
    Object o = val.extract<Object>();
    bind(o, pos);
  }
  else if ( val.type() == typeid(Array) )
  {
    Array arr = val.extract<Array>();
    bind(arr, pos);
  }
  else if ( val.type() == typeid(Timestamp) )
  {
    Timestamp time = val.extract<Timestamp>();
    bind(time, pos);
  }
  else if ( val.type() == typeid(DateTime) )
  {
    DateTime date = val.extract<DateTime>();
    bind(date, pos);
  }
}

} } } } // Namespace Poco::Script::JavaScript::SpiderMonkey
