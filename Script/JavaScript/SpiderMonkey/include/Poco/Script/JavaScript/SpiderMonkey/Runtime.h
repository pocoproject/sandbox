//
// Runtime.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Runtime
//
// Definition of Runtime.
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


#ifndef SpiderMonkey_Runtime_INCLUDED
#define SpiderMonkey_Runtime_INCLUDED

#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/SharedPtr.h"
#include "Poco/Mutex.h"

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class Context;
class ScriptContext;
class Environment;

class SpiderMonkey_API Runtime
	/// SpiderMonkey Runtime class. Encapsulates pointer to JSRuntime.
{
public:
	Runtime(Environment* env, int size);
		/// Creates and initializes a SpiderMonkey Runtime.

	virtual ~Runtime();
		/// Destroys SpiderMonkey Runtime.


	static SharedPtr<Context> createContext(SharedPtr<Runtime> rt, int size);
		/// Creates a context of the given size


private:
	Runtime(const Runtime&);


	Runtime& operator = (const Runtime&);


	operator JSRuntime*();


	Environment* _environment;


	JSRuntime* _rt;


	static Mutex _contextMutex;


	friend class Context;
	friend class Environment;
};


inline Runtime::operator JSRuntime*()
{
	return _rt;
}


} } } } // namespace Poco::Script::JavaScript::SpiderMonkey


#endif
