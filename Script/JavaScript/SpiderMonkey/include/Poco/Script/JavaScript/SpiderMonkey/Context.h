//
// Context.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Context
//
// Definition of Context.
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


#ifndef SpiderMonkey_Context_INCLUDED
#define SpiderMonkey_Context_INCLUDED

#include <Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h>

#include <Poco/URI.h>
#include <Poco/SharedPtr.h>

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class Array;
class Arguments;
class Object;

class SpiderMonkey_API Context
	/// SpiderMonkey Context class. Encapsulates pointer to JSContext.
{
public:

	Context(const Context&);


  virtual ~Context();
		/// Destroys SpiderMonkey Context.

  JSContext* operator *() const;


	Context& operator = (const Context&);


  bool isFree() const;
   /// Returns true when the context is not associated with a thread


  bool isValid() const;


  void setGlobalObject(const Object& object) const;


  void reportError(const std::string& error) const;


  void reportException(const Poco::Exception& e) const;


  Object getGlobalObject() const;


  void beginRequest() const;


  void endRequest() const;


  void clear() const;


  Object getActiveScript() const;


  Poco::URI resolveURI(const std::string& uri) const;


  Poco::URI resolveURI(const Poco::URI& uri) const;


  static Poco::URI createURI(const std::string& uri);


  void setPrivate(void* p) const;


  void* getPrivate() const;

private:


  Context(JSContext* cx);


  Context();


  void set(JSContext* cx);


	JSContext* _context;

	int _count;
	static int _counter;

  friend class Array;
  friend class Object;
  friend class Function;
  friend class ScriptContext;
  friend class RuntimeContext;

  template<class T_Port,
           class T_Priv,
           int ctor>
  friend class NativeClass;
};


inline JSContext* Context::operator *() const
{
  return _context;
}


inline bool Context::isValid() const
{
  return _context != NULL;
}


inline void Context::set(JSContext* cx)
{
  _context = cx;
}


inline bool Context::isFree() const
{
	return JS_GetContextThread(_context) == 0;
}


inline void Context::reportError(const std::string& error) const
{
  JS_ReportError(_context, error.c_str());
}


inline void Context::reportException(const Poco::Exception& e) const
{
  JS_ReportError(_context, e.displayText().c_str());
}


inline void Context::setPrivate(void *p) const
{
  JS_SetContextPrivate(_context, p);
}


inline void* Context::getPrivate() const
{
  return JS_GetContextPrivate(_context);
}

} } } } // namespace Poco::Script::JavaScript::SpiderMonkey


#endif
