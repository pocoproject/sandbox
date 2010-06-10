//
// Extractor.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Extractor
//
// Implementation of Extractor
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
#include <float.h>

#include "Poco/Script/ScriptException.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Extractor.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Function.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Array.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Date.h"

#include <jsdate.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

Extractor::Extractor(const Context& context, jsval* value) : _context(context), _value(value)
{
}


Extractor::~Extractor()
{
}


bool Extractor::extract(Poco::Int16& val, int pos)
{
  int32 i;
  if ( JS_ValueToInt32(*_context, _value[pos], &i) == JS_TRUE )
  {
    val = i;
    return true;
  }
  return false;
}


bool Extractor::extract(Poco::UInt16& val, int pos)
{
  return JS_ValueToUint16(*_context, _value[pos], &val) == JS_TRUE;
}


bool Extractor::extract(Poco::Int32& val, int pos)
{
  int32 i;
  if ( JS_ValueToECMAInt32(*_context, _value[pos], &i) == JS_TRUE )
  {
    val = i;
    return true;
  }

  return false;
}


bool Extractor::extract(Poco::UInt32& val, int pos)
{
  uint32 i;
  if ( JS_ValueToECMAUint32(*_context, _value[pos], &i) == JS_TRUE )
  {
    val = i;
    return true;
  }

  return false;
}


bool Extractor::extract(bool& val, int pos)
{
  if ( JSVAL_IS_BOOLEAN(_value[pos]) )
  {
    val = JSVAL_TO_BOOLEAN(_value[pos]) == JS_TRUE;
    return true;
  }
  return false;
}


bool Extractor::extract(float& val, int pos)
{
  jsdouble jsf;
  if ( JS_ValueToNumber(*_context, _value[pos], &jsf) == JS_TRUE )
  {
    if ( (jsf < 0 && jsf < FLT_MIN)
         || jsf > FLT_MAX )
    {
      return false;
    }

    val = (float) jsf;
    return true;
  }
  return false;
}


bool Extractor::extract(double& val, int pos)
{
  if ( JSVAL_IS_NUMBER(_value[pos]) )
  {
    jsdouble jsd;
    if ( JS_ValueToNumber(*_context, _value[pos], &jsd) == JS_TRUE )
    {
      val = jsd;
      return true;
    }
  }
  return false;
}


bool Extractor::extract(char& val, int pos)
{
  val = '\0';

  std::string str;
  extract(str, pos);
  if ( ! str.empty() )
  {
    val = str.at(0);
  }
  
  return true;
}


bool Extractor::extract(std::string& val, int pos)
{
  JSString *str = JS_ValueToString(*_context, _value[pos]);
  if ( str != NULL )
  {
    jschar* jsstr = JS_GetStringChars(str);
    size_t srclen = JS_GetStringLength(str);

    if ( srclen > 0 )
    {
      size_t dstlen = 0;
      if ( JS_EncodeCharacters(*_context, jsstr, srclen, NULL, &dstlen) )
      {
        char *dst = (char *) JS_malloc(*_context, dstlen);
        if (dst != NULL)
        {
          JS_EncodeCharacters(*_context, jsstr, srclen, dst, &dstlen);
          val.assign(dst, dstlen);
          JS_free(*_context, dst);
        }
      }
    }
  }
  return true;
}


bool Extractor::extract(Object& obj, int pos)
{
  if ( JSVAL_IS_OBJECT(_value[pos]) )
  {
    JSObject* jsobject = JSVAL_NULL;
    if ( JS_ValueToObject(*_context, _value[pos], &jsobject) == JS_TRUE )
    {
      obj.setObject(jsobject);
      return true;
    }
  }
  return false;
}


Poco::DynamicAny Extractor::any(int pos)
{
  Poco::DynamicAny result;

  JSType type = JS_TypeOfValue(*_context, _value[pos]);
  
  switch(type)
  {
  case JSTYPE_NUMBER:
    if ( JSVAL_IS_INT(_value[pos]) )
    {
      int i;
      if ( extract(i, pos) )
      {
        result = i;
      }
    }
    else
    {
      double d;
      if ( extract(d, pos) )
      {
        result = d;
      }
    }
    break;
  case JSTYPE_BOOLEAN:
    {
      bool b;
      if ( extract(b, pos) )
      {
        result = b;
      }
    }
    break;
  case JSTYPE_STRING:
    {
      std::string s;
      if ( extract(s, pos) )
      {
        result = s;
      }
    }
    break;
  case JSTYPE_OBJECT:
  case JSTYPE_FUNCTION:
    if ( JSVAL_IS_NULL(_value[pos]) )
    {
      // Empty DynamicAny
    }
    else
    {
      Object obj(_context);
      if ( extract(obj, pos) )
      {
        if ( obj.isFunction() )
        {
          result = Function(obj);
        }
        else if ( obj.isArray() )
        {
          result = Array(obj);
        }
        else if ( obj.isDate() )
        {
          result = Date(obj);
        }
        else
        {
          result = obj;
        }
      }
    }
    break;
  case JSTYPE_VOID:
    // Empty DynamicAny
    break;
  default:
    {
      std::string s;
      if ( extract(s, pos) )
      {
        result = s;
      }
    }
    break;
  }

  return result;
}


} } } } // Namespace Poco::Script::JavaScript::SpiderMonkey
