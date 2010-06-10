/*
 * GLUEscript - ModuleInterface.h
 *
 * Copyright (c) 2002-2010 Franky Braem and the GLUEscript project
 *
 * Project Info: http://gluescript.sourceforge.net
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301,
 * USA.
 *
 * $Id$
 */

#ifndef _SpiderMonkey_ModuleInterface_INCLUDED
#define _SpiderMonkey_ModuleInterface_INCLUDED

#include <string>
#include <map>

#include <Poco/Script/JavaScript/SpiderMonkey/Object.h>

namespace Poco
{
namespace Script
{
namespace JavaScript
{
namespace SpiderMonkey
{

// Interface that each module must implement so it can be loaded
// by our class loader.
class SpiderMonkey_API ModuleInterface
{
public:
  ModuleInterface();


  virtual ~ModuleInterface();


  virtual Object initialize(Object& obj) = 0;


  virtual std::string id() const = 0;
};


inline ModuleInterface::ModuleInterface()
{
}


inline ModuleInterface::~ModuleInterface()
{
}


} } } } // Poco::Script::JavaScript::SpiderMonkey

#endif // _SpiderMonkey_ModuleInterface_INCLUDED
