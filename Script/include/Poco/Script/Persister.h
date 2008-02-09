//
// Persister.h
//
// $Id: //poco/Main/Script/include/Poco/Script/Persister.h#1 $
//
// Library: Script
// Package: Core
// Module:  Persister
//
// Definition of Persister.
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


#ifndef Script_Persister_INCLUDED
#define Script_Persister_INCLUDED


#include "Poco/Script/Script.h"
#include "Poco/Script/Environment.h"
#include "Poco/BufferedBidirectionalStreamBuf.h"
#include <iostream>


namespace Poco {
namespace Script {


class Environment;


class Script_API Persister
	/// Persister class
{
public:
	Persister(Environment& rEnv, std::streamsize bufSize = 4096);
		/// Creates Script writer.

	~Persister();
		/// Destroys Script writer.

	void persist();
		/// Persists the environment into the internal buffer.

	void unpersist();
		/// Unpersists the environment from the internal buffer.

	void persist(Environment& env);
		/// Persists the supplied environment into the internal buffer.

	void unpersist(Environment& env);
		/// Unpersists the environment from the internal buffer.

	std::size_t getSize() const;

	void setSize(std::streamsize bufSize);

protected:

private:
	Persister();
	Persister(const Persister&);
	Persister& operator = (const Persister&);

	Environment&  _rEnv;
	std::streamsize _bufSize;
	BufferedBidirectionalStreamBuf _buf;
	std::iostream _stream;
};


inline std::size_t Persister::getSize() const
{
	return _bufSize;
}


inline void Persister::setSize(std::streamsize bufSize)
{
	_bufSize = bufSize;
}


} } // namespace Poco::Script::Script


#endif
