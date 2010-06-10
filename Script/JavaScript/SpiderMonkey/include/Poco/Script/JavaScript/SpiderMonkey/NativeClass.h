//
// NativeClass.h
//
// $Id$
//
// Library: JavaScript
// Package: SpiderMonkey
// Module:  NativeClass
//
// Definition of NativeClass.
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
#include <iostream>

#ifndef SpiderMonkey_NativeClass_INCLUDED
#define SpiderMonkey_NativeClass_INCLUDED

#include "Poco/Script/JavaScript/SpiderMonkey/SpiderMonkey.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Context.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Extractor.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Binder.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Object.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Function.h"
#include "Poco/Script/JavaScript/SpiderMonkey/Arguments.h"
#include "Poco/DynamicAny.h"

#include <jsapi.h>

namespace Poco {
namespace Script {
namespace JavaScript {
namespace SpiderMonkey {

template<class T_Port,
         class T_Priv,
         int ctor = 0>
class NativeClass : public Object
{
public:
  typedef NativeClass<T_Port, T_Priv, ctor> TOBJECT;

  // Pointer to a member function that will be called when a method
  // of the class is called in JavaScript
  typedef Poco::DynamicAny (T_Port::*MethodPtr)(T_Priv* p, Function& fun);


  // Pointer to a member function that will be called when a
  // class method (i.e. static) is called on a ported class
  typedef Poco::DynamicAny (T_Port::*ClassMethodPtr)(Function& fun);

  // A container with all pointers to member functions which are called
  // when a function of an object is called.
  // The current size is used by MethodCallback to retrieve the
  // method that must be executed (it's stored in a reserved slot)
  static std::vector<MethodPtr> _objectMethods;

  // A container with all pointers to member functions which are called
  // when a class method is called.
  // The current size is used by ClassMethodCallback to retrieve the
  // method that must be executed (it's stored in a reserved slot)
  static std::vector<ClassMethodPtr> _classMethods;

  // Constructor
  NativeClass(const Context& context, JSObject* obj) : Object(context, obj)
  {
  }

  NativeClass(const Poco::DynamicAny& any) : Object(any)
  {
  }


  operator const Poco::DynamicAny() const
  {
    return Object(*this);
  }


  Poco::DynamicAny any() const
  {
    return Object(*this);
  }

  
  static std::string getClassName()
  {
    return _jsClassName;
  }


  operator T_Priv* ()
  {
    return getPrivate();
  }

  // Create a new object of this class and assign the private pointer
  static Object createObject(const Context& context,
                             void *p = NULL)
  {
    return Object::newObject(context, &native_class, p, _classProto);
  }

  static bool instanceOf(const Object& obj)
  {
    Object walk = obj;
    // Rundown the prototype
    while( walk.isValid() )
    {
      JSClass* clazz = walk.getClass();
      if ( clazz == NULL )
        break;

      if ( clazz == &native_class )
      {
        return true;
      }

      walk = walk.getPrototype();
    }
    
    return false;
  }

  // Get the private value of this object. This function will check if 
  // this object is of the correct instance
  T_Priv* getPrivate() const
  {
    if ( instanceOf(*this) )
    {
      return (T_Priv*) Object::getPrivate();
    }
    return NULL;
  }

  // Initializes the class.
  static Object initializeClass(Object& global,
                                const Object& proto = Object::nullObject)
  {
    _classProto = Object(*global.context(),
                         JS_InitClass(*global.context(),
                                      *global,
                                      *proto,
                                      &native_class,
                                      T_Port::JSConstructor,
                                      ctor,
                                      NULL, NULL, NULL, NULL));
    if ( _classProto.isValid() )
    {
      T_Port::defineProperties(_classProto);
      T_Port::defineMethods(_classProto);

      Object ctorObject = _classProto.getConstructor();
      if ( ctorObject.isValid() )
      {
        T_Port::defineConstants(ctorObject);
        T_Port::defineClassProperties(ctorObject);
        T_Port::defineClassMethods(ctorObject);
      }
      T_Port::initClass(global, _classProto);

      _initialized = true;
    }
    return _classProto;
  }

  /**
   * Default implementation for adding a property
   */
  void addNativeProperty(T_Priv* p,
                         const std::string& prop,
                         const Poco::DynamicAny& v)
  {
  }

  /**
   * Default implementation for deleting a property
   */
  void deleteNativeProperty(T_Priv* p,
                            const std::string& prop)
  {
  }

  /**
   *  The default implementation of the Get method for a ported object.
   *  Overwrite this method when your object has properties.
   *  The default implementation returns JSVAL_VOID which means 'undefined'.
   */
  Poco::DynamicAny getNativeProperty(T_Priv* p,
                                     const Poco::DynamicAny& property)
  {
    return Poco::DynamicAny();
  }

  /**
   *  The default implementation of the Set method for a ported object.
   *  Overwrite this method when your object has properties.
   */
  void setNativeProperty(T_Priv* p,
                         const Poco::DynamicAny& property,
                         const Poco::DynamicAny& value)
  {
  }

  bool resolveNative(const Poco::DynamicAny& property)
  {
    return true;
  }

  /**
   *  The default implementation of the Destruct method. Overwrite this
   *  when you need to do some cleanup before the object is destroyed.
   *  The default implementation calls the destructor of the private
   *  object.
   */
  void destruct(T_Priv* p)
  {
    delete p;
    p = NULL;
  }

  /**
   *  The default implementation of the Construct method. Overwrite this
   *  when a script is allowed to create an object with the new statement.
   *  The default implementation returns NULL, which means that is not
   *  allowed to create an object of this class.
   */
  T_Priv* construct(Function& args)
  {
    return NULL;
  }

  /**
   * This method is called when the constructor of the class is called
   * without the new keyword. The default implementation returns
   * JSVAL_VOID which means 'undefined'.
   */
  Poco::DynamicAny callConstruct(Function& args)
  {
    return Poco::DynamicAny();
  }

  /**
   *  Default implementation for defining properties.
   *  Use the GLUE_DECLARE_PROPERTY_MAP, GLUE_BEGIN_PROPERTY_MAP and
   *  GLUE_END_PROPERTY_MAP macro's for hiding the complexity of
   *  defining properties. The default implementation does nothing.
   */
  static void defineProperties(const Object& obj)
  {
  }

  /**
   * initClass is called when the prototype object is created
   * It can be used for example to initialize constants related to
   * this class.
   * The argument obj is normally the global object.
   * The default implementation does nothing.
   */
  static void initClass(const Object& obj,
                        const Object& proto)
  {
  }

  /**
   *  Default implementation for defining methods.
   *  Use the GLUE_DECLARE_METHOD_MAP, GLUE_BEGIN_METHOD_MAP and
   *  GLUE_END_METHOD_MAP macro's for hiding the complexity of
   *  defining methods.
   *  The default implementation does nothing.
   */
  static void defineMethods(const Object& obj)
  {
  }

  /**
   *  Default implementation for defining constants.
   *  Use the GLUE_DECLARE_CONSTANT_MAP, GLUE_BEGIN_CONSTANT_MAP and
   *  GLUE_END_CONSTANT_MAP macro's for hiding the complexity of
   *  defining constants.
   *  The default implementation does nothing.
   *  Only numeric constants are allowed.
   */
  static void defineConstants(const Object& obj)
  {
  }

  /**
   *  Default implementation for defining static(class) properties.
   *  Use the GLUE_DECLARE_CLASS_PROPERTY_MAP,
   *  GLUE_BEGIN_CLASS_PROPERTY_MAP and GLUE_END_PROPERTY_MAP macro's
   *  for hiding the complexity of defining properties.
   *  The default implementation does nothing.
   */
  static void defineClassProperties(const Object& obj)
  {
  }

  /**
   *  Default implementation for defining static(class) methods.
   *  Use the GLUE_DECLARE_CLASS_METHOD_MAP, GLUE_BEGIN_CLASS_METHOD_MAP
   *  and GLUE_END_METHOD_MAP macro's for hiding the complexity of
   *  defining methods.
   *  The default implementation does nothing.
   */
  static void defineClassMethods(const Object& obj)
  {
  }

  /**
   *  The default implementation of the static Get method for a ported
   *  object. Overwrite this method when your object has static
   *  properties.
   *  The default implementation returns JSVAL_VOID (i.e. 'undefined').
   */
  Poco::DynamicAny getNativeClassProperty(const Poco::DynamicAny& property)
  {
    return Poco::DynamicAny();
  }

  /**
   *  The default implementation of the static Set method for a ported
   *  object.
   *  Overwrite this method when your object has static properties.
   */
  void setNativeClassProperty(const Poco::DynamicAny& property,
                              const Poco::DynamicAny& value)
  {
  }

  // Get the prototype of this class
  static Object& getClassPrototype()
  {
      return _classProto;
  }

  // The JS API callbacks

  static JSBool JSGetClassProperty(JSContext* cx,
                                   JSObject* obj,
                                   jsval id,
                                   jsval* vp)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    T_Port port(context, obj);
    Poco::DynamicAny property =  Extractor(context, &id).any();
    Poco::DynamicAny value = port.getNativeClassProperty(property);
    if ( ! value.isEmpty() )
    {
      Binder(context, vp).any(value);
    }
    return JS_TRUE;
  }

  static JSBool JSSetClassProperty(JSContext* cx,
                                   JSObject* obj,
                                   jsval id,
                                   jsval *vp)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    T_Port port(context, obj);

    Poco::DynamicAny property = Extractor(context, &id).any();
    Poco::DynamicAny value = Extractor(context, vp).any();
    port.setNativeClassProperty(property, value);
    return JS_TRUE;
  }

  // This method will redirect the call to a member function
  static JSBool MethodCallback(JSContext* cx,
                               JSObject* obj,
                               uintN argc,
                               jsval* argv,
                               jsval* rval)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    Object funObject(context);
    if ( Extractor(context, &argv[-2]).extract(funObject) )
    {
      Function f(funObject);
      for(unsigned int i = 0; i < argc; i++)
      {
        f.addArgument(Extractor(context, &argv[i]).any());
      }

      int methodIndex = f.getIndex();
      if ( methodIndex != -1 )
      {
        T_Port port(context, obj);
        MethodPtr callback = _objectMethods[methodIndex];
        T_Priv* p = port;
        if ( p != NULL )
        {
          // Protect all objects, strings, ... that are created in the callback
          JSAutoLocalRootScope localRootScope(cx);

          Poco::DynamicAny rv = (port.*callback)(p, f);
          Binder(context, rval).any(rv);
        }
      }
    }
    return JS_TRUE;
  }

  // This method will redirect a class method to a member function
  static JSBool ClassMethodCallback(JSContext* cx,
                                    JSObject* obj,
                                    uintN argc,
                                    jsval* argv,
                                    jsval* rval)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    Object funObject(context);
    if ( Extractor(context, &argv[-2]).extract(funObject) )
    {
      Function f(funObject);
      for(unsigned int i = 0; i < argc; i++)
      {
        f.addArgument(Extractor(context, &argv[i]).any());
      }
      int methodIndex = f.getIndex();
      if ( methodIndex != -1 )
      {
        T_Port port(context, obj);
        ClassMethodPtr callback = _classMethods[methodIndex];

        // Protect all objects, strings, ... that are created in the callback
        JSAutoLocalRootScope localRootScope(cx);

        Poco::DynamicAny rv = (port.*callback)(f);
        Binder(context, rval).any(rv);
      }
    }
    return JS_TRUE;
  }

protected:

  // Defines a function on the class prototype and store the pointer to
  // the member function into the container. Use the new size as the value
  // of the reserved slot, so MethodCallback can retrieve the pointer to
  // the member function
  static void addMethod(const Object& obj,
                        const char* name,
                        MethodPtr ptr,
                        uintN nargs)
  {
    JSFunction* fun
      = JS_DefineFunction(*obj.context(), *obj, name, MethodCallback, nargs,
                          JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE);
    if ( fun != NULL )
    {
      JSObject* funObject = JS_GetFunctionObject(fun);
      _objectMethods.push_back(ptr);
      jsval v = INT_TO_JSVAL(_objectMethods.size() - 1);
      JS_SetReservedSlot(*obj.context(), funObject, 0, v);
    }
  }

  // Defines a class function and store the pointer to
  // the member function into the container. Use the new size as the value
  // of the reserved slot, so MethodCallback can retrieve the pointer to
  // the member function
  static void addClassMethod(const Object& obj,
                             const char* name,
                             ClassMethodPtr ptr,
                             uintN nargs)
  {
    JSFunction* fun
      = JS_DefineFunction(*obj.context(), *obj, name, ClassMethodCallback, nargs,
                          JSPROP_READONLY | JSPROP_PERMANENT | JSPROP_ENUMERATE);
    if ( fun != NULL )
    {
      JSObject* funObject = JS_GetFunctionObject(fun);
      _classMethods.push_back(ptr);
      jsval v = INT_TO_JSVAL(_classMethods.size() - 1);
      JS_SetReservedSlot(*obj.context(), funObject, 0, v);
    }
  }

private:

  /**
   * The prototype object of the class
   */
  static Object _classProto;

  /**
   *  The name of the class.
   *  You can use the GLUE_INIT_CLASS macro, to initialize this.
   */
  static const char* _jsClassName;

  /**
   * The JSClass structure
   */
  static JSClass native_class;

  static bool _initialized;

  /**
   *  AddProperty callback. This will call the AddProperty method of
   *  the ported object.
   */
  static JSBool JSAddProperty(JSContext *cx,
                              JSObject *obj,
                              jsval id,
                              jsval *vp)
  {
    if ( ! _initialized )
      return JS_TRUE;

    Context context(cx);
    poco_assert_dbg(context.isValid());

    if (JSVAL_IS_STRING(id))
    {
      std::string property;
      if ( Extractor(context, &id).extract(property) )
      {
        T_Port port(context, obj);
        T_Priv *p = port;
        if ( p != NULL )
        {
          port.addNativeProperty(p, property, Extractor(context, vp).any());
        }
      }
    }
    return JS_TRUE;
  }

  /**
   *  AddProperty callback. This will call the AddProperty method of
   *  the ported object.
   */
  static JSBool JSDeleteProperty(JSContext *cx,
                                 JSObject *obj,
                                 jsval id,
                                 jsval* vp)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    if (JSVAL_IS_STRING(id))
    {
      T_Port port(context, obj);
      T_Priv *p = port;
      if ( p != NULL )
      {
        std::string property;
        if ( Extractor(context, &id).extract(property) )
        {
          port.deleteNativeProperty(p, property);
        }
      }
    }
    return JS_TRUE;
  }

  /**
   *  GetProperty callback. This will call the Get method of the
   *  ported object.
   */
  static JSBool JSGetProperty(JSContext *cx,
                              JSObject *obj,
                              jsval id,
                              jsval *vp)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    Poco::DynamicAny property = Extractor(context, &id).any();

    T_Port port(context, obj);
    T_Priv *p = port;
    if ( p == NULL )
    {
      return JS_TRUE;
    }

    if ( property.isString() )
    {
      std::string propertyName = property;
      // To be sure that we don't override a method of a class we have
      // to check if a property with the given name already exists
      jsval fun;
      JS_LookupProperty(cx, obj, propertyName.c_str(), &fun);
      if ( fun != JSVAL_VOID )
      {
         return JS_TRUE;
      }
    }

    JSAutoLocalRootScope localRootScope(cx);
    Poco::DynamicAny v = port.getNativeProperty(p, property);

    if ( ! v.isEmpty() )
    {
      Binder(context, vp).any(v);
    }

    return JS_TRUE;
  }

  /**
   *  SetProperty callback. This will call the Set method of the ported
   *  object.
   */
  static JSBool JSSetProperty(JSContext *cx,
                              JSObject *obj,
                              jsval id,
                              jsval *vp)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    Poco::DynamicAny property = Extractor(context, &id).any();
    Poco::DynamicAny value = Extractor(context, vp).any();

    T_Port port(context, obj);
    T_Priv *p = port;
    if ( p == NULL )
    {
      return JS_TRUE;
    }

    JSAutoLocalRootScope localRootScope(cx);
    port.setNativeProperty(p, property, value);

    return JS_TRUE;
  }

  /**
   *  Resolve callback. This will call the Resolve method of the ported
   *  object.
   */
  static JSBool JSResolve(JSContext *cx,
                          JSObject* obj,
                          jsval id,
                          uintN flags,
                          JSObject** objp)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    T_Port port(context, *objp);
    Poco::DynamicAny property = Extractor(context, &id).any();

    if ( ! port.resolveNative(property) )
    {
      *objp = NULL;
    }
    return JS_TRUE;
  }

  /**
   *  Constructor callback. This will call the Construct
   *  method of the ported object when 'new' is used (or the 'construct'
   *  method defined in the engine) or will call the CallConstruct
   *  method of the ported object when 'new' is omitted.
   */
  static JSBool JSConstructor(JSContext* cx,
                              JSObject* obj,
                              uintN argc,
                              jsval* argv,
                              jsval* rval)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    Object ctorObj(cx, JS_GetConstructor(cx, obj));
    Function ctorFn(ctorObj);

    unsigned int i;
    for(i = 0; i < argc; i++)
    {
      ctorFn[i] = Extractor(context, &argv[i]).any();
    }

    T_Port port(context, obj);

    JSClass* clazz = JS_GetClass(cx, obj);
    // It's necessary to check the class because calling the constructor from
    // our 'construct' method will fail JS_IsConstructing, but when the class
    // of obj is the same of the port we know 'construct' is called.
    if (    JS_IsConstructing(cx) == JS_TRUE
         || clazz == &native_class )
    {
      T_Priv *p = port.construct(ctorFn);
      if ( p == NULL )
      {
        JS_ReportError(cx, "Class %s can't be constructed", _jsClassName);
        return JS_FALSE;
      }
      JS_SetPrivate(cx, obj, p);
      return JS_TRUE;
    }

    Poco::DynamicAny any = port.callConstruct(ctorFn);
    if ( ! any.isEmpty() )
    {
      Binder(context, rval).any(any);
    }
    return JS_TRUE;
  }

  /**
   * Destructor callback. This will call the Destruct method of the
   * ported object.
   */
  static void JSDestructor(JSContext *cx, JSObject *obj)
  {
    Context context(cx);
    poco_assert_dbg(context.isValid());

    T_Port port(context, obj);
    T_Priv *p = port;
    if ( p != NULL )
    {
      port.destruct(p);
    }
  }
};

// The initialisation of native_class
template<class T_Port, class T_Priv, int ctor>
JSClass NativeClass<T_Port, T_Priv, ctor>::native_class =
{
    TOBJECT::_jsClassName,
    JSCLASS_HAS_PRIVATE
    /*| T_Enum::GetFlags() */
    | JSCLASS_HAS_RESERVED_SLOTS(2)
    | JSCLASS_NEW_RESOLVE
    | JSCLASS_NEW_RESOLVE_GETS_START,
    TOBJECT::JSAddProperty,
    TOBJECT::JSDeleteProperty,
    TOBJECT::JSGetProperty,
    TOBJECT::JSSetProperty,
    JS_EnumerateStub /*(JSEnumerateOp) T_Enum::JSEnumerate*/,
    (JSResolveOp) TOBJECT::JSResolve,
    JS_ConvertStub,
    TOBJECT::JSDestructor,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL,
    NULL
};

}}}}

// Some usefull macro's that makes the use of ApiWrapper easy
// PROPERTY MACROS
#define GLUE_NORMAL   JSPROP_ENUMERATE | JSPROP_PERMANENT
#define GLUE_READONLY JSPROP_ENUMERATE | JSPROP_PERMANENT | JSPROP_READONLY

// Declare initClass
#define GLUE_DECLARE_INIT_CLASS()                                      \
        static void initClass(const Object& obj, const Object& proto);

#define GLUE_BEGIN_INIT_CLASS(native)                \
        void native::initClass(const Object& obj,    \
                               const Object& proto)  \
        {

#define GLUE_END_INIT_CLASS() \
        }

// Declare a property map (use it in headers)
#define GLUE_DECLARE_PROPERTY_MAP() \
    static void defineProperties(const Object& obj);

// Declare a static property map (use it in headers)
#define GLUE_DECLARE_CLASS_PROPERTY_MAP() \
        static void defineClassProperties(const Object& obj);

// Begins a property map (use it in source files)
#define GLUE_BEGIN_PROPERTY_MAP(className)                  \
        void className::defineProperties(const Object& obj) \
        {                                                   \
          JSPropertySpec props[] =                          \
          {
// Ends a property map (use it in source files)
#define GLUE_END_PROPERTY_MAP()                             \
             { 0, 0, 0, 0, 0 }                              \
          };                                                \
          JS_DefineProperties(*obj.context(), *obj, props); \
        }
// Begins a static property map
#define GLUE_BEGIN_CLASS_PROPERTY_MAP(className)                 \
        void className::defineClassProperties(const Object& obj) \
        {                                                        \
          JSPropertySpec props[] =                               \
          {

// Defines a property
#define GLUE_PROPERTY(id, name)         \
        { name, id, GLUE_NORMAL, 0, 0 },

// Defines a static property
#define GLUE_CLASS_PROPERTY(id, name)                                     \
        { name, id, GLUE_NORMAL, JSGetClassProperty, JSSetClassProperty },

// Defines a readonly property
#define GLUE_READONLY_PROPERTY(id, name)   \
        { name, id, GLUE_READONLY, 0, 0 },

// Defines a readonly static property
#define GLUE_READONLY_CLASS_PROPERTY(id, name)             \
        { name, id, GLUE_READONLY, JSGetClassProperty, 0 },

// Declares a constant map
#define GLUE_DECLARE_CONSTANT_MAP() \
        static void defineConstants(const Object& obj);

// Begins a constant map
#define GLUE_BEGIN_CONSTANT_MAP(className)                 \
        void className::defineConstants(const Object& obj) \
        {                                                  \
          JSConstDoubleSpec consts[] =                     \
          {

// Ends a constant map
#define GLUE_END_CONSTANT_MAP()                               \
             { 0, 0, 0, { 0 } }                                \
          };                                                   \
          JS_DefineConstDoubles(*obj.context(), *obj, consts); \
        }

#define GLUE_BEGIN_CONSTANT_OBJECT(name)    \
        JSConstDoubleSpec name##Map[] =     \
        {
#define GLUE_END_CONSTANT_OBJECT(obj, name)                                  \
          { 0, 0, 0, { 0 } }                                                 \
        };                                                                   \
        JSObject* ct##name                                                   \
          = JS_DefineObject(*obj.context(), *obj, #name,                     \
                            NULL, NULL,                                      \
                            JSPROP_READONLY | JSPROP_PERMANENT);             \
        JS_DefineConstDoubles(*obj.context(), ct##name, name##Map);

// Defines a constant with a prefix
#define GLUE_CONSTANT(prefix, name)                     \
        { prefix##name, #name, GLUE_READONLY, { 0 } },

// Defines a constant
#define GLUE_SIMPLE_CONSTANT(name) { name, #name, GLUE_READONLY, { 0 } },

// METHOD MACROS
#define GLUE_DECLARE_METHOD_MAP()                     \
        static void defineMethods(const Object& obj);

#define GLUE_BEGIN_METHOD_MAP(className)                  \
        void className::defineMethods(const Object& obj)  \
        {
#define GLUE_END_METHOD_MAP() \
        }

#define GLUE_METHOD(name, method, args)      \
        addMethod(obj, name, &method, args);

#define GLUE_DECLARE_METHOD(name, priv)            \
        Poco::DynamicAny name(priv* p,             \
                              Function &fun);

#define GLUE_CLASS_METHOD(name, method, args) \
        addClassMethod(obj, name, &method, args);

#define GLUE_DECLARE_CLASS_METHOD_MAP() \
        static void defineClassMethods(const Object& obj);

#define GLUE_BEGIN_METHOD(native, priv, method)        \
        Poco::DynamicAny native::method(priv* p,       \
                                        Function& fun) \
        {

#define GLUE_END_METHOD(ret) \
          return ret;        \
        }

#define GLUE_END_METHOD_NO_RETURN()   \
          return Poco::DynamicAny();  \
        }

#define GLUE_BEGIN_CLASS_METHOD(native, method)              \
        Poco::DynamicAny native::method(Function& fun) \
        {
#define GLUE_BEGIN_CLASS_METHOD_NO_ARGS(native, method)       \
        Poco::DynamicAny native::method(Function& fun)  \
        {

#define GLUE_BEGIN_CLASS_METHOD_MAP(className)                \
        void className::defineClassMethods(const Object& obj) \
        {

#define GLUE_DECLARE_CLASS_METHOD(name)       \
        Poco::DynamicAny name(Function& fun);

#define GLUE_START_PROPERTY_ID -128

#define GLUE_DECLARE_GET_PROPERTY(priv)                                      \
        Poco::DynamicAny getNativeProperty(priv *p,                          \
                                           const Poco::DynamicAny& property);

#define GLUE_BEGIN_GET_PROPERTY(native, priv)                                        \
        Poco::DynamicAny native::getNativeProperty(priv* p,                          \
                                                   const Poco::DynamicAny& property) \
        {

#define GLUE_GET_PROPERTY(type, id, fun)         \
    case id:                                     \
      return Poco::DynamicAny(p->fun());

#define GLUE_END_GET_PROPERTY() \
           return Poco::DynamicAny();         \
        }

#define GLUE_DECLARE_SET_PROPERTY(priv)                          \
        void setNativeProperty(priv *p,                          \
                               const Poco::DynamicAny& property, \
                               const Poco::DynamicAny& value);

#define GLUE_BEGIN_SET_PROPERTY(native, priv)                            \
        void native::setNativeProperty(priv *p,                          \
                                       const Poco::DynamicAny& property, \
                                       const Poco::DynamicAny& value)    \
        {

#define GLUE_SET_PROPERTY(type, id, fun)      \
        case id:                              \
          {                                   \
            if ( ! value.isEmpty() )          \
            {                                 \
              p->fun(value);                  \
            }                                 \
            break;                            \
          }

#define GLUE_END_SET_PROPERTY() \
        }

#define GLUE_DECLARE_ADD_PROPERTY(priv)                        \
        void addNativeProperty(priv *p,                        \
                               const std::string &prop,        \
                               const Poco::DynamicAny& value);

#define GLUE_BEGIN_ADD_PROPERTY(native, priv)                         \
        void native::addNativeProperty(priv *p,                       \
                                       const std::string& prop,       \
                                       const Poco::DynamicAny& value) \
        {

#define GLUE_END_ADD_PROPERTY() \
        }

#define GLUE_DECLARE_DEL_PROPERTY(priv)                     \
        void deleteNativeProperty(priv *p,                  \
                                  const std::string &prop);

#define GLUE_BEGIN_DEL_PROPERTY(native, priv)                 \
        void native::deleteNativeProperty(priv *p,            \
                                    const std::string& prop)  \
        {

#define GLUE_END_DEL_PROPERTY() \
        }

#define GLUE_DECLARE_GET_CLASS_PROPERTY()     \
        Poco::DynamicAny getNativeClassProperty(Poco::DynamicAny& property);

#define GLUE_BEGIN_GET_CLASS_PROPERTY(native)        \
        Poco::DynamicAny native::getNativeClassProperty(Poco::DynamicAny& property)        \
        {

#define GLUE_GET_CLASS_PROPERTY(type, id, fun) \
    case id:                                     \
      return JavaScriptType<type>(cx, fun());

#define GLUE_SET_CLASS_PROPERTY(type, id, fun)   \
        case id:                                   \
          {                                        \
            fun(CppType<type>(cx, *vp));           \
            break;                                 \
          }

#define GLUE_DECLARE_SET_CLASS_PROPERTY()                             \
        void setNativeClassProperty(const Poco::DynamicAny& property, \
                                    const Poco::DynamicAny& value);

#define GLUE_BEGIN_SET_CLASS_PROPERTY(native)                                 \
        void native::setNativeClassProperty(const Poco::DynamicAny& property, \
                                            const Poco::DynamicAny& value)    \
        {

#define GLUE_DECLARE_CTOR(priv) \
        priv *construct(Function& fun);

#define GLUE_BEGIN_CTOR(native, priv)          \
        priv *native::construct(Function& fun) \
        {

#define GLUE_END_CTOR(priv) \
          return priv;      \
        }

#define GLUE_DECLARE_CALL_CTOR() \
        Poco::DynamicAny callConstruct(Function& fun);

#define GLUE_BEGIN_CALL_CTOR(native)                          \
        Poco::DynamicAny native::callConstruct(Function& fun) \
        {

#define GLUE_END_CALL_CTOR(ret) \
           return ret;          \
        }

#define GLUE_DECLARE_DTOR(priv)  \
        void destruct(priv* p);

#define GLUE_DONT_DESTROY(priv)  \
        void destruct(priv *p)   \
        {                        \
        }

#define GLUE_BEGIN_DESTROY(native, priv)   \
        void native::destruct(priv *p)     \
        {

#define GLUE_END_DESTROY() \
        }

#define GLUE_IMPL_NATIVE_CTOR(native)                                         \
        native(const Context& context, JSObject* obj) : TOBJECT(context, obj) \
        {                                                                     \
        }                                                                     \
        native(const Poco::DynamicAny& any) : TOBJECT(any)                    \
        {                                                                     \
        }

#define GLUE_DECLARE_RESOLVE() \
        bool resolveNative(const Poco::DynamicAny& property);

#define GLUE_BEGIN_RESOLVE(native)                                    \
        bool native::resolveNative(const Poco::DynamicAny& property)  \
        {
#define GLUE_END_RESOLVE() \
        }

// CLASS MACROS
#define GLUE_INIT_CLASS(type, name)                                         \
  template<>                                                                \
  Poco::Script::JavaScript::SpiderMonkey::Object type::TOBJECT::_classProto \
     = Object::nullObject;                                                  \
                                                                            \
  template<>                                                                \
  bool type::TOBJECT::_initialized = false;                                 \
                                                                            \
  template<>                                                                \
  const char* type::TOBJECT::_jsClassName = name;                           \
                                                                            \
  template<>                                                                \
  std::vector<type::TOBJECT::ClassMethodPtr> type::TOBJECT::_classMethods   \
    = std::vector<type::TOBJECT::ClassMethodPtr>();                         \
                                                                            \
  template<>                                                                \
  std::vector<type::TOBJECT::MethodPtr> type::TOBJECT::_objectMethods       \
    = std::vector<type::TOBJECT::MethodPtr>();

#endif // SpiderMonkey_NativeClass_INCLUDED
