//
// RuntimeContext.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  RuntimeContext
//
// Implementation of RuntimeContext
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
#include <sstream>
#include <iostream>

#include <Poco/Logger.h>

#include <Poco/Script/JavaScript/SpiderMonkey/RuntimeContext.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {


RuntimeContext::RuntimeContext(SharedPtr<Runtime>& runtime, int size) : _runtime(runtime), _context(NULL)
{
	_context = JS_NewContext(*runtime, size);
	if ( _context != NULL )
	{
		JS_SetErrorReporter(_context, &RuntimeContext::errorReporter);
		JS_BeginRequest(_context);
		JS_SetOptions(_context, JSOPTION_XML | JSOPTION_VAROBJFIX);
		JS_SetVersion(_context, JSVERSION_1_8);

		//if ( m_configuration->GetBranchLimit() > 0 )
		//{
		//  JS_SetBranchCallback(cx, Engine::BranchCallback);
		//  JS_ToggleOptions(cx, JSOPTION_NATIVE_BRANCH_CALLBACK);
		//}
		JS_EndRequest(_context);
	}
}


RuntimeContext::~RuntimeContext()
{
	JS_SetContextThread(_context);
	JS_ClearNewbornRoots(_context);
	JS_ClearRegExpStatics(_context);
	JS_ClearPendingException(_context);
	JS_MaybeGC(_context);
	JS_DestroyContext(_context);
}


void RuntimeContext::errorReporter(JSContext *cx, const char *message, JSErrorReport *report)
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


} } } }
