//
// Environment.h
//
// $Id: //poco/Main/Script/include/Poco/Script/Environment.h#1 $
//
// Library: Script
// Package: Core
// Module:  Environment
//
// Definition of Environment.
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


#ifndef Script_Environment_INCLUDED
#define Script_Environment_INCLUDED


#include "Poco/Script/Script.h"
#include "Poco/Script/Persister.h"
#include "Poco/DynamicAny.h"
#include <vector>
#include <iostream>


namespace Poco {
namespace Script {


class Script_API Environment
	/// Environment class
{
public:
	Environment();
		/// Creates Script environment.

	virtual ~Environment();
		/// Destroys Script environment.

	virtual std::vector<Poco::DynamicAny>* execute(const std::string& code, 
		const std::vector<DynamicAny>* pArguments = 0, 
		std::vector<Poco::DynamicAny>* pResults = 0) = 0;
		/// Executes supplied code.
		/// If arguments vector is supplied, arguments are passed to the
		/// execution environment.
		/// If results vector is supplied, results are filled in and returned.

	virtual int read(char* buf, int length) = 0;
		/// Reads the stream into the environment.

	virtual int write(const char* buf, int length) = 0;
		/// Writes the environment into the stream.

	virtual void reset();
		/// Resets the environment.

protected:
	std::vector<char> _buffer;

private:
	Environment(const Environment&);
	Environment& operator = (const Environment&);
};


///
/// inlines
///
inline void Environment::reset()
{
}


} } // namespace Poco::Script


#endif
