//
// Persister.cpp
//
// $Id: //poco/Main/Script/src/Persister.cpp#1 $
//
// Library: Script
// Package: Core
// Module:  Persister
//
// Implementation of Persister
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


#include "Poco/Script/Persister.h"
#include <ios>


namespace Poco {
namespace Script {


Persister::Persister(Environment& rEnv, std::streamsize bufSize): 
	_rEnv(rEnv),
	_bufSize(bufSize),
	_buf(bufSize, std::ios::in | std::ios::out),
	_stream(&_buf)
{
}


Persister::~Persister()
{
}


inline void Persister::persist()
{
	//_rEnv.write(_stream);
}


inline void Persister::unpersist()
{
	//_rEnv.read(_stream);
}


inline void Persister::persist(Environment& env)
{
	//env.write(_stream);
}


inline void Persister::unpersist(Environment& env)
{
	//env.read(_stream);
}


} } // namespace Poco::Script
