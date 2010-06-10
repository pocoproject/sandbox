//
// Object.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Object
//
// Implementation of Object
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
#include <iostream>

#include <Poco/StringTokenizer.h>

#include "Poco/Script/ScriptException.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Binder.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Extractor.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Function.h"

#include <Poco/Path.h>


namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {


Object Object::nullObject = Object();

Object::Object(const Poco::DynamicAny& any) : _obj(JSVAL_NULL)
                                            , _context(NULL)
{
  if ( any.type() == typeid(Object) )
  {
    *this = any.extract<Object>();
  }
}


Object::Object(const Context& cx, JSObject* obj) : _obj(obj)
                                                 , _context(cx)
{
}


Object::Object(const Object& copy) : _obj(copy._obj)
                                   , _context(copy._context)
{
}


Object::Object() : _obj(JSVAL_NULL)
{
}

Object::~Object()
{
}


Object& Object::operator = (const Object& other)
{
  if (&other != this)
  {
    _context = other._context;
    _obj = other._obj;
  }
  return *this;
}


Object& Object::operator = (const Poco::DynamicAny& any)
{
  if ( any.type() == typeid(Object) )
  {
    *this = any.extract<Object>();
  }
  return *this;
}


void Object::attachData(void* p, int slot)
{
  JS_SetReservedSlot(*_context, _obj, slot, PRIVATE_TO_JSVAL(p));
}


void* Object::getAttachedData(int slot)
{
  jsval v;
  if ( JS_GetReservedSlot(*_context, _obj, slot, &v) == JS_TRUE )
  {
    return JSVAL_TO_PRIVATE(v);
  }
  return NULL;
}


void Object::setProperty(const Poco::DynamicAny& property,
                         const Poco::DynamicAny& value) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  jsval v = JSVAL_NULL;
  Binder b(_context, &v);
  b.any(value);

  if ( property.isInteger() )
  {
    JS_SetElement(*_context, _obj, property, &v);
  }
  else
  {
    std::string name = property;
    JS_SetProperty(*_context, _obj, name.c_str(), &v);
  }
}


void Object::defineProperty(const std::string& property,
                            const DynamicAny& value,
                            bool readonly) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  uintN attrs = JSPROP_PERMANENT | JSPROP_ENUMERATE;
  if ( readonly )
  {
    attrs |= JSPROP_READONLY;
  }

  jsval v = JSVAL_NULL;
  Binder(_context, &v).any(value);
  JS_DefineProperty(*_context, _obj, property.c_str(), v, NULL, NULL, attrs);
}


void Object::deleteProperty(const std::string& property) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JS_DeleteProperty(*_context, _obj, property.c_str());
}

DynamicAny Object::getProperty(const Poco::DynamicAny& property) const
{
  if ( ! isValid() )
    return Poco::DynamicAny();

  jsval v = JSVAL_NULL;
  if ( property.isInteger() )
  {
    JS_GetElement(*_context, _obj, property, &v);
  }
  else
  {
    std::string name = property;
    JS_GetProperty(*_context, _obj, name.c_str(), &v);
  }

  Poco::DynamicAny result = Extractor(_context, &v).any();
  return result;
}


DynamicAny Object::lookupProperty(const std::string& property) const
{
  poco_assert_dbg(_context.isValid());

  if ( _obj == JSVAL_NULL)
  {
    return Poco::DynamicAny();
  }

  jsval rval;
  if (JS_LookupProperty(*_context, _obj, property.c_str(), &rval) == JS_FALSE)
  {
    return Poco::DynamicAny();
  }
  Poco::DynamicAny result = Extractor(_context, &rval).any();
  return result;
}


bool Object::hasProperty(const Poco::DynamicAny& property) const
{
  poco_assert_dbg(_context.isValid());

  if ( _obj == JSVAL_NULL)
  {
    return false;
  }

  JSBool found = JS_FALSE;
/* Only available in JS 1.8
  if ( property.isInteger() )
  {
    jsid id = property;
    JS_HasPropertyById(*_context, _obj, id, &found);
  }
  else
*/
  {
    std::string name = property;
    JS_HasProperty(*_context, _obj, name.c_str(), &found);
  }

  return found == JS_TRUE;
}


void Object::enumerateProperties(std::map<Poco::DynamicAny,
                                 Poco::DynamicAny>& properties) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JSIdArray* ida = JS_Enumerate(*_context, _obj);
  if ( ida != NULL )
  {
    for (jsint i=0; i < ida->length; i++)
    {
      jsval vIdx;
      if ( JS_IdToValue(*_context, ida->vector[i], &vIdx) == JS_TRUE )
      {
        Poco::DynamicAny property =  Extractor(_context, &vIdx).any();

        Poco::DynamicAny value = getProperty(property);
        properties[property] = value;
      }
    }

    JS_DestroyIdArray(*_context, ida);
  }
}


bool Object::execute(const std::string& source,
                     const std::string& filename) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JSScript *jsscript = JS_CompileScript(*_context,
                                        _obj,
                                        source.c_str(),
                                        source.length(),
                                        filename.c_str(),
                                        0);
  if ( jsscript == NULL )
  {
    JS_ReportPendingException(*_context);
    return false;
  }

  Object objScript(_context, JS_NewScriptObject(*_context, jsscript));
  defineProperty("__script__", objScript, true);
  objScript.setProperty("filename", filename);

  Poco::URI scriptURI = Context::createURI(filename);
  objScript.setProperty("uri", scriptURI.toString());

  std::vector<std::string> paths;
  scriptURI.getPathSegments(paths);
  paths.pop_back();
  std::string newPath;
  for(std::vector<std::string>::iterator it = paths.begin(); it != paths.end(); it++)
  {
    newPath.append("/");
    newPath.append(*it);
  }
  newPath.append("/");
  scriptURI.setPath(newPath);
  objScript.setProperty("path", scriptURI.toString());

  Object objParentScript = _context.getActiveScript();
  if ( objParentScript.isValid() )
  {
    objScript.setProperty("parent", objParentScript);
  }

  jsval vresult;
  JSBool ok = JS_ExecuteScript(*_context, _obj, jsscript, &vresult);
  JS_SetProperty(*_context, *objScript, "result", &vresult);

  if ( ok == JS_FALSE )
  {
    JS_ReportPendingException(*_context);
    return false;
  }
  return true;
}


void Object::setParent(const Object& parent) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JS_SetParent(*_context, _obj, *parent);
}


Object Object::getParent() const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  return Object(_context, JS_GetParent(*_context, _obj));
}


void Object::setPrivate(void* p) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JS_SetPrivate(*_context, _obj, p);
}


void* Object::getPrivate() const
{
  if ( ! _context.isValid() )
    return NULL;

  if ( _obj != JSVAL_NULL )
  {
    return JS_GetPrivate(*_context, _obj);
  }
  else
  {
    return NULL;
  }
}


Object Object::getPrototype() const
{
  poco_assert_dbg(_context.isValid());

  if ( _obj == JSVAL_NULL )
    return nullObject;

  return Object(_context, JS_GetPrototype(*_context, _obj));
}


void Object::setPrototype(const Object& proto) const
{
  poco_assert_dbg(_context.isValid());

  if ( _obj != JSVAL_NULL )
  {
    JS_SetPrototype(*_context, _obj, *proto);
  }
}


bool Object::hasPrototype(const std::string& className) const
{
  if ( ! _context.isValid() )
    return false;

  if ( _obj == NULL )
    return false;

  jsval ctor, proto;

  if (JS_LookupProperty(*_context, JS_GetGlobalObject(*_context), className.c_str(), &ctor) == JS_FALSE)
    return false;

  if (JS_LookupProperty(*_context, JSVAL_TO_OBJECT(ctor), "prototype", &proto) == JS_FALSE)
    return false;

  JSObject *protoObj = JSVAL_TO_OBJECT(proto);
  JSClass* jsclass = JS_GET_CLASS(*_context, protoObj);

  if ( jsclass == NULL )
      return false;

  JSObject *prototype = JS_GetPrototype(*_context, _obj);
  while(    prototype != NULL
       && JS_InstanceOf(*_context, prototype, jsclass, NULL) == JS_FALSE )
  {
    prototype = JS_GetPrototype(*_context, prototype);
  }
  return prototype != NULL;
}

JSClass* Object::getClass() const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  return JS_GetClass(*_context, _obj);
}


Object Object::getConstructor() const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  return Object(_context, JS_GetConstructor(*_context, _obj));
}


DynamicAny Object::call(Function& fn) const
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  if ( ! fn.isValid() )
  {
    return Poco::DynamicAny();
  }

  jsval rval = JSVAL_NULL;

  int argc = fn.arguments().size();
  jsval* argv = NULL;
  if ( argc > 0 )
  {
    argv = new jsval[argc];
    Binder b(_context, argv);
    for(int i = 0; i < argc; i++)
    {
      b.any(fn.arguments()[i], i);
    }
  }

  JS_CallFunctionValue(*_context, _obj, OBJECT_TO_JSVAL(fn._obj),
                       argc, argv, &rval);
  if ( JS_IsExceptionPending(*_context) )
  {
    JS_ReportPendingException(*_context);
  }

  if ( argc > 0 )
  {
    delete[] argv;
  }

  return Extractor(_context, &rval).any();
}

Object Object::newSimpleObject(const Context& context,
                               const Object& parent)
{
  return Object(context, JS_NewObject(*context, NULL, NULL, *parent));
}


Object Object::newObject(const Context& context,
                         const std::string& className,
                         void* priv)
{
  Arguments args;
  return newObject(context, className, priv, args);
}


Object Object::newObject(const Context& context,
                         JSClass* clazz,
                         void* priv,
                         const Object& proto,
                         const Object& parent)
{
  JSObject* obj = JS_NewObject(*context, clazz, *proto, *parent);
  if (    obj != NULL
       && (clazz->flags & JSCLASS_HAS_PRIVATE) == JSCLASS_HAS_PRIVATE )
  {
    JS_SetPrivate(*context, obj, priv);
  }
  return Object(context, obj);
}


Object Object::newObject(const Context& context,
                         const std::string& className,
                         void* priv,
                         Arguments& args)
{
  std::string funName = className;

  JSObject* namespaceObject = NULL;
  JSObject* currentObject   = JS_GetGlobalObject(*context);

  Poco::StringTokenizer tokenizer(className,
                                  ".",
                                  Poco::StringTokenizer::TOK_TRIM);
  for (Poco::StringTokenizer::Iterator it = tokenizer.begin();
       it != tokenizer.end();
       ++it)
  {
    jsval prop;
    namespaceObject = currentObject;
    if ( JS_GetProperty(*context,
                        currentObject,
                        it->c_str(),
                        &prop) ==  JS_FALSE )
    {
      return Object();
    }

    if ( ! JSVAL_IS_OBJECT(prop) )
    {
      return Object();
    }

    currentObject = JSVAL_TO_OBJECT(prop);
    funName = *it;
  }

  jsval proto;
  if ( JS_LookupProperty(*context,
                         currentObject,
                         "prototype",
                         &proto) == JS_FALSE )
  {
    return Object();
  }
  if ( ! JSVAL_IS_OBJECT(proto) )
  {
    return Object();
  }

  currentObject = JSVAL_TO_OBJECT(proto);

  JSObject* newObj = JS_NewObject(*context,
                                  JS_GetClass(*context, currentObject),
                                  currentObject,
                                  NULL);
  if ( priv == NULL ) // Call construct to create a private
  {
    JSObject* nextObj;
    jsval vfun;
    if ( JS_GetMethod(*context,
                      namespaceObject,
                      funName.c_str(),
                      &nextObj,
                      &vfun) == JS_TRUE )
    {
      uintN argc = args.size();
      jsval* argv = new jsval[argc];
      Binder  b(context, argv);
      for(uint i = 0; i < argc; i++)
      {
        b.any(args[i], i);
      }
      jsval rval;
      JS_CallFunctionValue(*context, newObj, vfun, argc, argv, &rval);
      delete[] argv;
    }
  }
  else
  {
    JS_SetPrivate(*context, newObj, priv);
  }
  return Object(context, newObj);
}


void Object::root()
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JSClass* clazz = JS_GetClass(*_context, _obj);
  JS_AddNamedRoot(*_context, &_obj, clazz->name);
}


void Object::unroot()
{
  poco_assert_dbg(_context.isValid());
  poco_assert_dbg(_obj != JSVAL_NULL);

  JS_RemoveRoot(*_context, &_obj);
}

} } } }
