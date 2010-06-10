//
// Function.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Function
//
// Implementation of Function
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

#include "Poco/Script/JavaScript/SpiderMonkey/Function.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Extractor.h"

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

Function::Function(const Poco::DynamicAny& any) : Object()
{
  if ( any.type() == typeid(Function) )
  {
    *this = any.extract<Function>();
  }
}


Function::Function(const Object& obj) : Object(obj)
{
}


Function::Function(const Poco::Script::JavaScript::SpiderMonkey::Object& obj, const std::string &name) : Object()
{
  jsval property;
  if ( JS_GetProperty(*obj.context(), *obj, name.c_str(), &property) == JS_TRUE )
  {
    if ( JSVAL_IS_OBJECT(property) )
    {
      JSObject* objFn = JSVAL_TO_OBJECT(property);
      if ( JS_ObjectIsFunction(*obj.context(), objFn) )
      {
        _obj = objFn;
        _context = obj.context();
      }
    }
  }
}


Function::Function(const Function& copy) : Object(copy), _arguments(copy._arguments)
{
}


Function& Function::operator = (const Function& other)
{
  if (&other != this) 
  {
    Object::operator =(other);
  }
  return *this;
}


Function::~Function()
{
}


bool Function::isValid() const
{
  if ( Object::isValid() )
  {
    return JS_ObjectIsFunction(*_context, _obj) == JS_TRUE;
  }
  return false;
}


void Function::addArgument(const DynamicAny& arg)
{
  _arguments.add(arg);
}


Arguments& Function::arguments()
{
  return _arguments;
}


int Function::getIndex()
{
  int index = -1;
  jsval methodIndex;
  if ( JS_GetReservedSlot(*_context, _obj, 0, &methodIndex) == JS_TRUE )
  {
    Extractor(_context, &methodIndex).extract(index);
  }
  return index;
}


Poco::DynamicAny& Function::operator[](int index)
{
  return _arguments[index];
}


std::string Function::name() const
{
  JSFunction* fun = (JSFunction*) getPrivate();
  JSString* str = JS_GetFunctionId(fun);
  std::string result;

  if ( str != NULL )
  {
    jschar* jsstr = JS_GetStringChars(str);
    size_t srclen = JS_GetStringLength(str);

    if ( srclen > 0 )
    {
      size_t dstlen = 0;
      if ( JS_EncodeCharacters(*context(), jsstr, srclen, NULL, &dstlen) )
      {
        char *dst = (char *) JS_malloc(*context(), dstlen);
        if (dst != NULL)
        {
          JS_EncodeCharacters(*context(), jsstr, srclen, dst, &dstlen);
          result.assign(dst, dstlen);
          JS_free(*context(), dst);
        }
      }
    }
  }
  return result;

}


} } } }
