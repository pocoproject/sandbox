//
// Environment.cpp
//
// $Id: //poco/Main/Script/Lua/src/Environment.cpp#1 $
//
// Library: Lua
// Package: Lua
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


#include "Poco/Script/Lua/Environment.h"
#include "Poco/Script/Lua/lua/lauxlib.h"
#include "Poco/Script/Lua/lua/lualib.h"
#include "Poco/Script/ScriptException.h"
#include "Poco/Any.h"


using Poco::Script::ScriptException;
using Poco::Any;
using Poco::AnyCast;


namespace Poco {
namespace Script {
namespace Lua {


Environment::Environment()
{
	init();
}


Environment::Environment(const Environment& other)
{
	*this = other;
}

	
Environment& Environment::operator = (const Environment& other)
{
	if (&other != this) 
	{
		init();
		_buffer = other._buffer;
	}
	return *this;
}


Environment::~Environment()
{
	lua_close(_pState);
}


void Environment::init()
{
	_pState = lua_open();
	poco_assert(_pState);
	luaL_openlibs(_pState);
#if 0
	luaopen_pluto(_pState);
#endif
}


void Environment::reset()
{
	Storage().swap(_buffer);
	lua_settop(_pState, 0);
}


void Environment::execute(const std::string& code)
{
	int error = luaL_loadbuffer(_pState, code.c_str(), code.size(), "line");
	if (error) handleError();

	error = lua_pcall(_pState, 0, LUA_MULTRET, 0);
	if (error) handleError();
}


void Environment::call(const std::string& code, int argc, int ret)
{
	int error = 0;

	if (!code.empty())
	{
		error = luaL_loadstring(_pState, code.c_str());
		if (error) handleError();
	}

	error = lua_pcall(_pState, argc, ret, 0);
	if (error) handleError();
}


std::vector<Any>* Environment::execute(const std::string& code, 
	const std::vector<Any>* pArguments, 
	std::vector<Any>* pResults)
{
	call(code);
	
	int argCount = 0;

	if (pArguments)
	{
		argCount = static_cast<int>(pArguments->size());
		if (argCount) pushValues(*pArguments);
	}

	int errFunc = 0;
	int error = lua_pcall(_pState, argCount, pResults ? LUA_MULTRET : 0, errFunc);
	if (error) handleError();
	if (pResults) popValues(*pResults);

	return pResults;
}


int Environment::read(char* buf, int length)
{
#if 0
	pluto_unpersist(_pState, reader, this);
	if (length > _buffer.size())
		length = static_cast<int>(_buffer.size());
	memcpy(buf, &_buffer[0], length);
#endif
	return length;
}


int Environment::write(const char* buf, int length)
{
#if 0
	std::string str(buf, length);
	call(str);
	pluto_persist(_pState, writer, this);
#endif
	return static_cast<int>(_buffer.size());
}


int Environment::writer(lua_State* pL, const void* p, size_t sz, void* ud)
{
	Environment* pEnv = static_cast<Environment*>(ud);
	std::vector<char>::size_type oldSize = pEnv->_buffer.size();
	pEnv->_buffer.resize(oldSize + sz);
	memcpy(&pEnv->_buffer[oldSize], p, sz);
	return 0;
}


const char* Environment::reader(lua_State *L, void *ud, size_t *sz)
{
	Environment* pEnv = static_cast<Environment*>(ud);
	*sz = pEnv->_buffer.size();
	return &pEnv->_buffer[0];
}


void Environment::pushValues(const std::vector<Poco::Any>& values)
{
	std::vector<Any>::const_iterator it = values.begin();
	std::vector<Any>::const_iterator end = values.end();
	for(; it != end; ++it)
	{
		if (it->type() == typeid(String))
			lua_pushstring(_pState, RefAnyCast<String>(*it).c_str());
		else if (it->type() == typeid(Integer))
			lua_pushinteger(_pState, RefAnyCast<lua_Integer>(*it));
		else if (it->type() == typeid(Number))
			lua_pushnumber(_pState, RefAnyCast<lua_Number>(*it));
	}
}


void Environment::popValues(std::vector<Poco::Any>& values)
{
	int stackSize;
	while ((stackSize = lua_gettop(_pState)))
	{
		switch (lua_type(_pState, stackSize))
		{
			case LUA_TTABLE://todo
				break;
			case LUA_TNUMBER:
				values.push_back(lua_tonumber(_pState, stackSize));
				break;
			case LUA_TSTRING: 
				values.push_back(String(lua_tostring(_pState, stackSize)));
				break;
			default: 
				throw ScriptException("Unknown or unsupported return value encountered.");
		}

		lua_pop(_pState, 1);
	}
}


void Environment::handleError()
{
	throw ScriptException(std::string(lua_tostring(_pState, -1)));
}


} } } // namespace Poco::Script::Lua
