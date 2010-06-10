//
// Extractor.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Extractor
//
// Definition of Extractor.
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


#ifndef SpiderMonkey_Extractor_INCLUDED
#define SpiderMonkey_Extractor_INCLUDED


#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/DynamicAny.h"

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class SpiderMonkey_API Extractor
  /// Extracts a C++ type from a JavaScript type
{
public:
  Extractor(const Context& context, jsval* value);
   /// Default Constructor


  virtual ~Extractor();
   /// Destructor

	
	virtual bool extract(Poco::Int16& val, int pos = 0);
		/// Extracts an Int16.
    
		
	virtual bool extract(Poco::UInt16& val, int pos = 0);
		/// Extracts an UInt16.
    
		
	virtual bool extract(Poco::Int32& val, int pos = 0);
		/// Extracts an Int32.
    
		
	virtual bool extract(Poco::UInt32& val, int pos = 0);
		/// Extracts an UInt32.
    
		
	virtual bool extract(bool& val, int pos = 0);
		/// Extracts a boolean.
    
		
	virtual bool extract(float& val, int pos = 0);
		/// Extracts a float.
    
		
	virtual bool extract(double& val, int pos = 0);
		/// Extracts a double.
    
		
	virtual bool extract(char& val, int pos = 0);
		/// Extracts a single character.
    

	virtual bool extract(std::string& val, int pos = 0);
		/// Extracts a string.


	virtual bool extract(Object& obj, int pos = 0);
		/// Extracts an object.


  virtual Poco::DynamicAny any(int pos = 0);
    
    
private:

  Context _context;


  jsval *_value;
  
  
};

} } } } // Namespace Poco::Script::JavaScript::SpiderMonkey

#endif // SpiderMonkey_Extractor_INCLUDED
