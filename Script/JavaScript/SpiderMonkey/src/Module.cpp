/*
 * GLUEscript - Module.cpp
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

#include <Poco/Script/JavaScript/SpiderMonkey/Object.h>
#include <Poco/Script/JavaScript/SpiderMonkey/Module.h>

namespace Poco
{
namespace Script
{
namespace JavaScript
{
namespace SpiderMonkey
{

Module::Module(const std::string& dllName) : _loaded(false)
                                           , _dllName(dllName)
                                           , _moduleItf(NULL)
{
  _classLoader.loadLibrary(_dllName);
  _loaded = true;

  const Poco::ClassLoader<ModuleInterface>::Manif* pManifest = _classLoader.findManifest(_dllName);
  poco_check_ptr(pManifest);
  Poco::ClassLoader<ModuleInterface>::Manif::Iterator it = pManifest->begin();
  Poco::ClassLoader<ModuleInterface>::Manif::Iterator itEnd = pManifest->end();
  for (; it != itEnd; ++it)
  {
    if (it->canCreate())
    {
      _moduleItf = it->create();
    }
  }
}

Module::~Module()
{
  if ( _moduleItf != NULL )
  {
    delete _moduleItf;
  }

  if ( isLoaded() )
  {
    _classLoader.unloadLibrary(_dllName);
  }
}

Object Module::init(Object& obj)
{
  if ( _moduleItf == NULL )
  {
    return Object::nullObject;
  }
  return _moduleItf->initialize(obj);
}


std::string Module::id() const
{
  if ( _moduleItf == NULL )
  {
    return "";
  }
  return _moduleItf->id();
}


} } } } // Namespace Poco::Script::JavaScript::SpiderMonkey
