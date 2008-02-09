//
// Environment.h
//
// $Id: //poco/Main/Script/Lua/include/Poco/Script/Lua/Environment.h#1 $
//
// Library: Lua
// Package: Lua
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


#ifndef Lua_Environment_INCLUDED
#define Lua_Environment_INCLUDED


#define LUA_LIB
#include "Poco/Script/Environment.h"
#include "Poco/Script/Lua/Lua.h"
#include "Poco/Script/Lua/lua/lua.hpp"
#include "Poco/Any.h"
#include <vector>


namespace Poco {
namespace Script {
namespace Lua {


int writer(lua_State* pL, const void* p, size_t sz, void* ud);
const char* reader(lua_State *L, void *ud, size_t *sz);


class Lua_API Environment: public Poco::Script::Environment
	/// Lua environment class
{
public:
	typedef std::vector<char> Storage;
	typedef std::string       String;
	typedef lua_Integer       Integer;
	typedef lua_Number        Number;

	Environment();
		/// Creates and initializes Lua environment.

	Environment(const Environment&);
		/// Copy constructor.
	
	Environment& operator = (const Environment&);
		/// Assignment operator.

	~Environment();
		/// Destroys Lua environment.

	void newTable();

	void pushString(const std::string& str);

	std::string toString(int index);

	void pushNumber(Number number);

	Number toNumber(int index);

	void pushValue(int index);

	void setTable(int index);

	void getField(int index, const std::string& name);

	void call(const std::string& code = "", int argc = 0, int ret = LUA_MULTRET);

	void execute(const std::string& code);
		/// Executes supplied chunk of code.

	std::vector<Poco::Any>* execute(const std::string& code, 
		const std::vector<Any>* pArguments, 
		std::vector<Poco::Any>* pResults = 0);
		/// Executes supplied code with arguments and returns values in supplied vector
		/// if pointer is different than 0. Uses error function stack index if different than 0.

	int read(char* buf, int length);
		/// Reads the environment into the string.

	int write(const char* buf, int length);
		/// Writes the string into the environment.

	void reset();
		/// Resets the stack and clears the buffer.

private:
	void init();

	static int writer(lua_State* pL, const void* p, size_t sz, void* ud);
		/// The write function used by lua_dump. 
		/// Every time it produces another piece of chunk, lua_dump calls the writer, 
		/// passing along the buffer to be written (p), its size (sz), and the data parameter supplied to lua_dump.
		/// The writer returns an error code: 0 means no errors; any other value means an error and 
		/// stops lua_dump from calling the writer again. 

	static const char* reader(lua_State *L, void *ud, size_t *sz);
		/// The reader function used by lua_load. 
		/// Every time it needs another piece of the chunk, lua_load calls the reader, 
		/// passing along its data parameter. 
		/// The reader must return a pointer to a block of memory with a new piece of the chunk 
		/// and set size to the block size. 
		/// The block must exist until the reader function is called again. 
		/// To signal the end of the chunk, the reader must return NULL. 
		/// The reader function may return pieces of any size greater than zero.

	void pushValues(const std::vector<Poco::Any>& values);
	
	void popValues(std::vector<Poco::Any>& values);
	
	void handleError();

	lua_State* _pState;
	Storage _buffer;
};


///
/// inlines
///

inline void Environment::newTable()
{
	lua_newtable(_pState);
}


inline void Environment::pushString(const std::string& str)
{
	lua_pushstring(_pState, str.c_str());
}


inline std::string Environment::toString(int index)
{
	return std::string(lua_tostring(_pState, index));
}


inline Environment::Number Environment::toNumber(int index)
{
	return lua_tonumber(_pState, index);
}


inline void Environment::pushNumber(Number number)
{
	lua_pushnumber(_pState, number);
}


inline void Environment::pushValue(int index)
{
	lua_pushvalue(_pState, index); 
}


inline void Environment::setTable(int index)
{
	lua_settable(_pState, index); 
}


inline void Environment::getField(int index, const std::string& name)
{
	lua_getfield(_pState, index, name.c_str());
}


} } } // namespace Poco::Script::Lua


#endif
