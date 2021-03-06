//
// Context.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  RuntimeContext
//
// Definition of RuntimeContext.
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
#ifndef SpiderMonkey_RuntimeContext_INCLUDED
#define SpiderMonkey_RuntimeContext_INCLUDED

#include <Poco/SharedPtr.h>
#include <Poco/Script/JavaScript/SpiderMonkey/Runtime.h>
#include <Poco/Script/JavaScript/SpiderMonkey/Context.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class Runtime;

class SpiderMonkey_API RuntimeContext
  /// A class that is responsible for creating and destroying a context
{
public:
  RuntimeContext(SharedPtr<Runtime>& runtime, int size);


  virtual ~RuntimeContext();


  static void errorReporter(JSContext *cx, const char *message, JSErrorReport *report);


  Context getContext();


private:

  SharedPtr<Runtime> _runtime;

  JSContext* _context;

};


inline Context RuntimeContext::getContext()
{
	return _context;
}

} } } }


#endif // SpiderMonkey_RuntimeContext_INCLUDED
