//
// Object.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  Object
//
// Definition of Object.
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


#ifndef SpiderMonkey_Object_INCLUDED
#define SpiderMonkey_Object_INCLUDED

#include <map>

#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/SharedPtr.h"
#include "Poco/DynamicAny.h"

#include <jsapi.h>
#include <jsdate.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

class Function;
class Arguments;

class SpiderMonkey_API Object
  /// SpiderMonkey Object class. Encapsulates pointer to JSObject.
{
public:

  Object(const Poco::DynamicAny& any);
    /// Constructor. Creates an Object class from a DynamicAny


  Object(const Context& context, JSObject* obj = JSVAL_NULL);
    /// Constructor


  Object(const Object&);
    /// Copy Constructor


  Object& operator = (const Object&);


  Object& operator = (const Poco::DynamicAny& any);


  virtual ~Object();
    /// Destructor


  void attachData(void* p, int slot = 0);
    /// Attaches data to the object (calls JS_SetReservedSlot)


  void* getAttachedData(int slot = 0);
    /// Returns the data that was attached to the given slot

  virtual bool isValid() const;
    /// Returns true when the object is valid

  bool isFunction() const;
    /// Returns true when the object is a function

  bool isArray() const;
    /// Returns true when the object is an Array

  bool isDate() const;
    /// Returns true when the object is a Date

  void setProperty(const Poco::DynamicAny& property, const Poco::DynamicAny& value) const;
    /// Set the value of the property.

  void defineProperty(const std::string& property, const Poco::DynamicAny& value, bool readonly = false) const;
    /// Defines a property on the object

  void deleteProperty(const std::string& property) const;
    /// Deletes a property

  DynamicAny getProperty(const Poco::DynamicAny& property) const;
    /// Returns the value of a property.

  Poco::DynamicAny lookupProperty(const std::string& name) const;
    /// Lookup a property

  bool hasProperty(const Poco::DynamicAny& property) const;
    /// Returns true when the object has a property with the given name or index.

  DynamicAny call(Function& fn) const;
    /// Calls the function on this object

  void setParent(const Object& parent) const;
    /// Sets the parent of the object

  Object getParent() const;
    /// Returns the parent of the object

  void setPrivate(void* p) const;
    /// Sets the private data of the object

  void* getPrivate() const;
    /// Returns the private data

  Object getPrototype() const;
    /// Returns the prototype of the object

  void setPrototype(const Object& proto) const;
    /// Sets the prototype

  JSClass* getClass() const;
    /// Returns the JSClass structure of the class of this object

  Object getConstructor() const;
    /// Returns the constructor function of the object

  bool hasPrototype(const std::string& className) const;
    /// Returns true when the object has a prototype of the given classname

  void enumerateProperties(std::map<Poco::DynamicAny, Poco::DynamicAny>& properties) const;
    /// Enumerate all properties of the class

  bool execute(const std::string& source, const std::string& filename) const;
    /// Executes a script with this object as scope

  void setObject(JSObject* obj);


  JSObject* operator *() const;


  const Context& context() const;


  Context& context();


  void setContext(const Context& context);


  static Object nullObject;


  void root();
    /// Protect the object against garbage collection

  void unroot();
    /// Remove the protection of the object

  static Object newSimpleObject(const Context& context, const Object& parent = Object::nullObject);
    /// Create a JavaScript 'Object'

  static Object newObject(const Context& context, const std::string& className, void* priv = NULL);
    /// Create an object of the given class

  static Object newObject(const Context& context, const std::string& className, void* priv, Arguments &args);
    /// Create an object of the given class and pass the arguments to the
    /// constructor

  static Object newObject(const Context& context,
                          JSClass* clazz,
                          void* priv = NULL,
                          const Object& prototype = Object::nullObject,
                          const Object& parent = Object::nullObject);
    /// Create an object of the given class

protected:

  JSObject* _obj;


  Context _context;


  Object();

};

inline bool Object::isValid() const
{
  return _obj != JSVAL_NULL;
}


inline bool Object::isFunction() const
{
  return _obj != JSVAL_NULL && JS_ObjectIsFunction(*_context, _obj);
}


inline bool Object::isArray() const
{
  return _obj != JSVAL_NULL && JS_IsArrayObject(*_context, _obj);
}


inline bool Object::isDate() const
{
  return _obj != JSVAL_NULL && js_DateIsValid(*_context, _obj);
}


inline JSObject* Object::operator *() const
{
  return _obj;
}


inline void Object::setObject(JSObject* obj)
{
  _obj = obj;
}


inline const Context& Object::context() const
{
  return _context;
}

inline Context& Object::context()
{
  return _context;
}


inline void Object::setContext(const Context& context)
{
  _context = context;
}

} } } } // namespace Poco::Script::JavaScript::SpiderMonkey

namespace Poco
{
template <>
class DynamicAnyHolderImpl<Script::JavaScript::SpiderMonkey::Object>: public DynamicAnyHolder
{
public:
	DynamicAnyHolderImpl(const Script::JavaScript::SpiderMonkey::Object& val): _val(val)
	{
    if ( _val.isValid() )
    {
      _val.root();
    }
	}

	~DynamicAnyHolderImpl()
	{
    if ( _val.isValid() )
    {
      _val.unroot();
    }
	}

	const std::type_info& type() const
	{
		return typeid(Script::JavaScript::SpiderMonkey::Object);
	}

	void convert(Int8&) const
	{
		throw BadCastException();
	}

	void convert(Int16&) const
	{
		throw BadCastException();
	}

	void convert(Int32&) const
	{
		throw BadCastException();
	}

	void convert(Int64&) const
	{
		throw BadCastException();
	}

	void convert(UInt8&) const
	{
		throw BadCastException();
	}

	void convert(UInt16&) const
	{
		throw BadCastException();
	}

	void convert(UInt32&) const
	{
		throw BadCastException();
	}

	void convert(UInt64&) const
	{
		throw BadCastException();
	}

	void convert(bool&) const
	{
		throw BadCastException();
	}

	void convert(float&) const
	{
		throw BadCastException();
	}

	void convert(double&) const
	{
		throw BadCastException();
	}

	void convert(char&) const
	{
		throw BadCastException();
	}

	void convert(std::string& s) const
	{
    if ( *_val == JSVAL_NULL )
    {
      s.clear();
      return;
    }

    JSString *str = JS_ValueToString(*_val.context(), OBJECT_TO_JSVAL(*_val));
    if ( str != NULL )
    {
      jschar* jsstr = JS_GetStringChars(str);
      size_t srclen = JS_GetStringLength(str);

      if ( srclen > 0 )
      {
        size_t dstlen = 0;
        if ( JS_EncodeCharacters(*_val.context(), jsstr, srclen, NULL, &dstlen) )
        {
          char *dst = (char *) JS_malloc(*_val.context(), dstlen);
          if (dst != NULL)
          {
            JS_EncodeCharacters(*_val.context(), jsstr, srclen, dst, &dstlen);
            s.assign(dst, dstlen);
            JS_free(*_val.context(), dst);
          }
        }
      }
    }
	}

	void convert(DateTime& val) const
	{
		//TODO: val = _val;
	}

	void convert(LocalDateTime& ldt) const
	{
		//TODO: ldt = _val.timestamp();
	}

	void convert(Timestamp& ts) const
	{
		//TODO: ts = _val.timestamp();
	}

	DynamicAnyHolder* clone() const
	{
		return new DynamicAnyHolderImpl(_val);
	}

	const Script::JavaScript::SpiderMonkey::Object& value() const
	{
		return _val;
	}

	bool isArray() const
	{
		return false;
	}

	bool isInteger() const
	{
		return false;
	}

	bool isSigned() const
	{
		return false;
	}

	bool isNumeric() const
	{
		return false;
	}

	bool isString() const
	{
		return false;
	}

private:
	Script::JavaScript::SpiderMonkey::Object _val;
};


}; // namespace Poco

#endif
