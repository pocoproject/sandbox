//
// JSDelegate.h
//
// $Id: //poco/Main/WebWidgets/include/Poco/WebWidgets/JSDelegate.h#4 $
//
// Library: WebWidgets
// Package: WebEvents
// Module:  JSDelegate
//
// Definition of the JSDelegate class.
//
// Copyright (c) 2007, Applied Informatics Software Engineering GmbH.
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


#ifndef WebWidgets_JSDelegate_INCLUDED
#define WebWidgets_JSDelegate_INCLUDED


#include "Poco/WebWidgets/WebWidgets.h"


namespace Poco {
namespace WebWidgets {


class WebWidgets_API JSDelegate
	/// A JSDelegate adds a javascript method call to a JavaScriptEvent
{
public:
	JSDelegate(const std::string& fct, const std::string& file);
		/// Creates the JSDelegate.

	virtual ~JSDelegate();
		/// Destroys the JSDelegate.

	const std::string& functionName() const;
		/// The JavaScript function to call. It is assumed that the 
		/// function takes one parameter, which is of type JavaScript EventArgs

	const std::string& jsFile() const;
		/// The javascript file we need to import so that
		/// the function is available

	bool operator<(const JSDelegate& del) const;

private:
	std::string _functionName;
	std::string _jsFile;
};


inline const std::string& JSDelegate::functionName() const
{
	return _functionName;
}


inline const std::string& JSDelegate::jsFile() const
{
	return _jsFile;
}


static JSDelegate jsDelegate(const std::string& fct, const std::string& file);


inline JSDelegate jsDelegate(const std::string& fct, const std::string& file)
{
	return JSDelegate(fct, file);
}


} } // namespace Poco::WebWidgets


#endif // WebWidgets_JSDelegate_INCLUDED
