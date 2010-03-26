//
// Context.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Context
//
// Implementation of Context
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

#include <Poco/Logger.h>

#include "Poco/Script/ScriptException.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Binder.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Arguments.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Array.h"

#include <sstream>

JSClass globalClass =
{
  "global", JSCLASS_HAS_PRIVATE,
  JS_PropertyStub,  JS_PropertyStub,
  JS_PropertyStub, JS_PropertyStub,
  JS_EnumerateStub, JS_ResolveStub,
  JS_ConvertStub,  JS_FinalizeStub,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL,
  NULL
};


namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

Context::Context(SharedPtr<Runtime> runtime, int size) : _runtime(runtime)
{
	_context = JS_NewContext(*runtime, size);
	if ( _context == NULL )
	{
		throw ScriptException("Can't create Context");
	}

  JS_SetErrorReporter(_context, &Context::errorReporter);

	JS_BeginRequest(_context);
	JS_SetOptions(_context, JSOPTION_XML);
	JS_SetVersion(_context, JSVERSION_1_7);
	JS_SetErrorReporter(_context, Context::errorReporter);

    //if ( m_configuration->GetBranchLimit() > 0 )
    //{
    //  JS_SetBranchCallback(cx, Engine::BranchCallback);
    //  JS_ToggleOptions(cx, JSOPTION_NATIVE_BRANCH_CALLBACK);
    //}
	JS_EndRequest(_context);
}


Context::Context() : _context(NULL)
{
}


Context::Context(JSContext* cx) : _context(cx)
{
}

Context::Context(const Context& context) : _context(context._context)
{
}


Context& Context::operator=(const Context& c)
{
  if ( this != &c )
  {
    _context = c._context;
  }
  return *this;
}


Context::~Context()
{
  if (      _context != NULL
       && ! _runtime.isNull() )
  {
    JS_SetContextThread(_context);
    clear();
    JS_DestroyContext(_context);
    _context = NULL;
  }
}


Object Context::getGlobalObject() const
{
	JSObject* obj = JS_GetGlobalObject(_context);
	if ( obj == NULL )
  {
	  obj = JS_NewObject(_context, &globalClass, 0, 0);
	  if ( obj != NULL )
	  {
		  JS_InitStandardClasses(_context, obj);
      return Object(*this, obj);
	  }
  }
  else
	{
		return Object(*this, obj);
	}

  return Object::nullObject;
}

void Context::setGlobalObject(const Object& object) const
{
	JS_SetGlobalObject(_context, *object);
}


void Context::errorReporter(JSContext *cx, const char *message, JSErrorReport *report)
{
  std::stringstream msg;

  if ( report->filename == NULL )
  {
    msg << message;
  }
  else
  {
    msg << report->filename;
    msg << "(";
    msg << report->lineno + 1;
    msg << ") : ";
    msg << message;
  }

  Poco::Logger& logger = Poco::Logger::get("js");
  logger.error(msg.str());
}


void Context::beginRequest() const
{
  JS_SetContextThread(_context);
  JS_BeginRequest(_context);
}


void Context::endRequest() const
{
  JS_EndRequest(_context);
  JS_ClearContextThread(_context);
}


void Context::clear() const
{
  JS_ClearNewbornRoots(_context);
  JS_ClearRegExpStatics(_context);
  JS_ClearPendingException(_context);
  //JS_GC(_context);
  JS_MaybeGC(_context);
}


Object Context::getActiveScript() const
{
  if ( JS_IsRunning(_context) )
  {
    Object scope(*this, JS_GetScopeChain(_context));
    while( scope.isValid() )
    {
      std::map<Poco::DynamicAny, Poco::DynamicAny> properties;
      if ( scope.hasProperty("__script__") )
      {
        Object script = scope.getProperty("__script__");
        return script;
      }
      scope = scope.getParent();
    }
  }
  return Object::nullObject;
}

Poco::URI Context::resolveURI(const std::string& uri) const
{
  return resolveURI(Context::createURI(uri));
}

Poco::URI Context::resolveURI(const Poco::URI &uri) const
{
  Poco::URI resolved = uri;
  if ( resolved.isRelative() )
  {
    Object script = getActiveScript();
    if ( script.isValid() )
    {
      Poco::DynamicAny path = script.getProperty("path");
      if ( ! path.isEmpty() )
      {
        Poco::URI scriptPathUri(path.convert<std::string>());
        scriptPathUri.resolve(resolved);
        resolved = scriptPathUri;
      }
    }
  }
  return resolved;
}

Poco::URI Context::createURI(const std::string& uri)
{
  Poco::URI realUri;
  try
  {
    std::string newUri(uri);
    std::replace(newUri.begin(), newUri.end(), '\\', '/' );
    realUri = newUri;

    #ifdef WIN32
      // When an absolute filepath (without a scheme) is given,
      // URI thinks that the drive letter is a scheme.
      if ( realUri.getScheme().length() == 1 )
      {
        realUri.setPath("/" + realUri.getScheme() + ":" + realUri.getPath());
        realUri.setScheme("file");
      }
    #endif

    if ( realUri.getScheme().empty() )
    {
      if (    realUri.getPath().length() > 0
           && realUri.getPath().at(0) == '/' )
      {
        realUri.setScheme("file");
      }
    }
  }
  catch(Poco::SyntaxException se)
  {
  }
  return realUri;
}

} } } }
