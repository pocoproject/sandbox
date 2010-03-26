//
// Binder.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Binder
//
// Definition of Binder.
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


#ifndef SpiderMonkey_Binder_INCLUDED
#define SpiderMonkey_Binder_INCLUDED


#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/DynamicAny.h"

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class Function;

class SpiderMonkey_API Binder
  /// Binds a C++ type to a JavaScript type
{
public:
  Binder(const Context& context, jsval* value);
   /// Default Constructor


  virtual ~Binder();
   /// Destructor


	virtual void bind(const Poco::Int16& val, int pos = 0);
		/// Binds an Int16.


	virtual void bind(const Poco::UInt16& val, int pos = 0);
		/// Binds an UInt16.


	virtual void bind(const Poco::Int32& val, int pos = 0);
		/// Binds an Int32.


	virtual void bind(const Poco::UInt32& val, int pos = 0);
		/// Binds an UInt32.


	virtual void bind(const bool& val, int pos = 0);
		/// Binds a boolean.


	virtual void bind(const float& val, int pos = 0);
		/// Binds a float.


	virtual void bind(const double& val, int pos = 0);
		/// Binds a double.


	virtual void bind(const char& val, int pos = 0);
		/// Binds a single character.


	virtual void bind(const std::string& val, int pos = 0);
		/// Binds a string.


  virtual void bind(const std::vector<Poco::DynamicAny> array, int pos = 0);
    /// Binds an array of dynamic values


  virtual void any(const Poco::DynamicAny& any, int pos = 0);
    /// Binds a DynamicAny


	virtual void bind(const Object& obj, int pos = 0);
		/// Binds an object.


  virtual void bind(const Function& val, int pos);
    /// Binds a Function


  virtual void bind(const Timestamp& stamp, int pos);
    /// Binds a Timestamp


  void bind(const DateTime& date, int pos);
    /// Binds a DateTime


private:

  Context _context;


  jsval *_value;


};

} } } } // Namespace Poco::Script::JavaScript::SpiderMonkey

#endif // SpiderMonkey_Binder_INCLUDED
