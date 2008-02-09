//
// Persister.h
//
// $Id: //poco/Main/Script/Lua/include/Poco/Script/Lua/Persister.h#1 $
//
// Library: Lua
// Package: Lua
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


#ifndef Lua_Persister_INCLUDED
#define Lua_Persister_INCLUDED


#define LUA_LIB
#include "Poco/Script/Lua/Lua.h"
#include "Poco/Script/Lua/lua/lua.h"
#include "Poco/Script/Lua/lua/pluto.h"
#include "Poco/Script/Lua/Environment.h"
#include <vector>


namespace Poco {
namespace Script {
namespace Lua {


class Lua_API Persister
	/// Persister class
{
public:

	Persister(Poco::Script::Lua::Environment& rEnv);
		/// Creates Lua writer.

	~Persister();
		/// Destroys Lua writer.

	void persist(std::vector<char>* pBuffer)
	{
		_pBuffer = pBuffer;
		poco_check_ptr(_pBuffer);
		//reset();
		pluto_persist(_rEnv, write, 0);
	}

	void unpersist(std::vector<char>* pBuffer)
	{
		_pBuffer = pBuffer;
		poco_check_ptr(_pBuffer);
		pluto_unpersist(_rEnv, read, 0);
	}

	static int write(lua_State* pL, const void* p, size_t sz, void* ud)
		/// The write function used by lua_dump. 
		/// Every time it produces another piece of chunk, lua_dump calls the writer, 
		/// passing along the buffer to be written (p), its size (sz), and the data parameter supplied to lua_dump.
		/// The writer returns an error code: 0 means no errors; any other value means an error and 
		/// stops lua_dump from calling the writer again. 
	{
		std::vector<char>::size_type oldSize = _pBuffer->size();
		_pBuffer->resize(oldSize + sz);
		memcpy(&(*_pBuffer)[oldSize], p, sz);
		return 0;
	}

	static const char* read(lua_State *L, void *ud, size_t *sz)
		/// The reader function used by lua_load. 
		/// Every time it needs another piece of the chunk, lua_load calls the reader, 
		/// passing along its data parameter. 
		/// The reader must return a pointer to a block of memory with a new piece of the chunk 
		/// and set size to the block size. 
		/// The block must exist until the reader function is called again. 
		/// To signal the end of the chunk, the reader must return NULL. 
		/// The reader function may return pieces of any size greater than zero.
	{
		*sz = _pBuffer->size();
		return &(*_pBuffer)[0];
	}
/*
	static void reset()
	{
		std::vector<char>().swap(_buffer);
	}
*/
private:
	Persister(const Persister&);
	Persister& operator = (const Persister&);

	static std::vector<char>* _pBuffer;
	Poco::Script::Lua::Environment& _rEnv;
};


} } } // namespace Poco::Script::Lua


#endif
