//
// Environment.cpp
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Environment
//
// Implementation of Environment
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
#include "Poco/Script/JavaScript/SpiderMonkey/Environment.h"
#include "Poco/Script/JavaScript/SpiderMonkey/ScriptContext.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Runtime.h"


namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {


Environment::Environment(int rtSize,
                         int ctxSize,
                         int poolSize) : _rtSize(rtSize)
                                       , _ctxSize(ctxSize)
                                       , _poolSize(poolSize)
{
	init();
}


Environment::~ Environment()
{
}


void Environment::init()
{
  system(); // Just call system to make sure our static variable is created
	createRuntime();

  _contextPool = new ContextPool(_rt, _poolSize);
}


void Environment::execute(const std::string& code)
{
	std::vector<DynamicAny> result;
	execute(code, NULL, &result);
}

Poco::SharedPtr<Context> Environment::getContext()
{
  return _contextPool->getContext();
}

std::vector<DynamicAny>* Environment::execute(const std::string& code,
	const std::vector<DynamicAny>* pArguments,
	std::vector<DynamicAny>* pResults)
{
	SharedPtr<Context> context = getContext();

	SharedPtr<ScriptContext> scriptContext = new ScriptContext(context);

  if ( pResults == NULL )
  {
    pResults = new std::vector<DynamicAny>();
  }

	try
	{
		 pResults = scriptContext->execute(code, pArguments, pResults);
	}
	catch(ScriptException e)
	{
	}

	return pResults;
}


void Environment::createRuntime()
{
  if ( _rt.isNull() )
  {
    _rt = new Runtime(this, _rtSize);
  }
}


System& Environment::system()
{
  static System system;
  return system;
}


} } } } // namespace Poco::Script::JavaScript::SpiderMonkey
