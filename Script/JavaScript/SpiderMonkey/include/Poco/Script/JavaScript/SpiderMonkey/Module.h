/*
 * GLUEscript - module.h
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

#ifndef SpiderMonkey_Module_INCLUDED
#define SpiderMonkey_Module_INCLUDED

#include <string>

#include <Poco/SharedLibrary.h>
#include <Poco/ClassLoader.h>

#include <Poco/Script/JavaScript/SpiderMonkey/Object.h>
#include <Poco/Script/JavaScript/SpiderMonkey/ModuleInterface.h>
#include <Poco/Script/JavaScript/SpiderMonkey/NativeClass.h>

namespace Poco
{
namespace Script
{
namespace JavaScript
{
namespace SpiderMonkey
{


class SpiderMonkey_API Module
{
public:
  Module(const std::string& dllName);
  
  
  virtual ~Module();

  
  bool isLoaded() const;


  Object init(Object& obj);


  std::string id() const;


private:
  Poco::ClassLoader<ModuleInterface> _classLoader;
  
  
  bool _loaded;
  
  
  std::string _dllName;
  
  
  ModuleInterface* _moduleItf;


  friend class System;
};


inline bool Module::isLoaded() const 
{
  return _loaded;
}


} } } }// Namespace Poco::Script::JavaScript::SpiderMonkey

#endif //SpiderMonkey_Module_INCLUDED
