//
// DefaultHandler.h
//
// $Id$
//
// Library: JSON
// Package: JSON
// Module:  DefaultHandler
//
// Definition of the DefaultHandler class.
//
// Copyright (c) 2012, Applied Informatics Software Engineering GmbH.
// and Contributors.
//
// Permission is hereby granted, free of charge, to any person or organization
// obtaining a copy of the software and accompanying documentation covered by
// this license (the "Software") to use, reproduce, display, distribute,
// execute, and transmit the Software, and to prepare derivative works of the
// Software, and to permit third-parties to whom the Software is furnished to
// do so, all subject to the following:
//
// The copyright notices in the Software and this entire statement, including
// the above license grant, this restriction and the following disclaimer,
// must be included in all copies of the Software, in whole or in part, and
// all derivative works of the Software, unless such copies or derivative
// works are solely in the form of machine-executable object code generated by
// a source language processor.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE, TITLE AND NON-INFRINGEMENT. IN NO EVENT
// SHALL THE COPYRIGHT HOLDERS OR ANYONE DISTRIBUTING THE SOFTWARE BE LIABLE
// FOR ANY DAMAGES OR OTHER LIABILITY, WHETHER IN CONTRACT, TORT OR OTHERWISE,
// ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
// DEALINGS IN THE SOFTWARE.
//

#ifndef JSON_DefaultHandler_INCLUDED
#define JSON_DefaultHandler_INCLUDED

#include "Poco/JSON/Handler.h"

#include <stack>

namespace Poco {
namespace JSON {

class JSON_API DefaultHandler : public Handler
{
public:


  DefaultHandler();


  virtual ~DefaultHandler();


  void startObject();


  void endObject();


  void startArray();


  void endArray();


  void key(const std::string& k);


  void value(const DynamicAny& value);


  DynamicAny result() const;


private:

  std::stack<DynamicAny> _stack;

  std::string _key;

  DynamicAny _result;
};


inline DynamicAny DefaultHandler::result() const
{
  return _result;
}

} } // Namespace Poco::JSON

#endif // JSON_DefaultHandler_INCLUDED