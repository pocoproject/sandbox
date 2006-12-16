/// ActionRequest.cpp


/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.
  

#ifndef Portlet_ActionRequest_INCLUDED
#define Portlet_ActionRequest_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
#ifndef Portlet_PortletRequest_INCLUDED
#include "Portlet/PortletRequest.h"
#endif
#ifndef Servlet_ServletInputStream_INCLUDED
#include "Servlet/ServletInputStream.h"
#endif
#include "Servlet/BufferedReader.h"

Portlet_BEGIN


class CPP_PORTLET_DLL_API ActionRequest : public PortletRequest
	/// The ActionRequest represents the request sent to the portlet
	/// to handle an action.
	/// It extends the PortletRequest interface to provide action request
	/// information to portlets.<br>
	/// The portlet container creates an ActionRequest object and
	/// passes it as argument to the portlet's processAction method.
{
public:
	typedef Servlet::ServletInputStream InputStream;
   
	virtual InputStream getPortletInputStream () = 0;
		/// Retrieves the body of the HTTP request from client to
		/// portal as binary data using
		/// an InputStream. Either this method or 
		/// {@link #getReader} may be called to read the body, but not both.
		/// 
		/// For HTTP POST data of type application/x-www-form-urlencoded
		/// this method throws an IllegalStateException
		/// as this data has been already processed by the 
		/// portal/portlet-container and is available as request parameters.
		///
		/// Returns an input stream containing the body of the request
		///
		/// Throws IllegalStateException if getReader was already called, 
		/// or it is a HTTP POST data of type application/x-www-form-urlencoded
		///
		/// Throws IOException if an input or output exception occurred
   

	virtual void setCharacterEncoding(const std::string& enc) = 0;
		/// Overrides the name of the character encoding used in the body of this
		/// request. This method must be called prior to reading input 
		/// using {@link #getReader} or {@link #getPortletInputStream}.
		/// 
		/// This method only sets the character set for the Reader that the
		/// {@link #getReader} method returns.
		///
		/// Parameter enc is a string containing the name of the 
		/// chararacter encoding.
		///
		/// Throws UnsupportedEncodingException if this is not a valid encoding
		/// Throws IllegalStateException if this method is called after 
		/// reading request parameters or reading input using  getReader()
   
  
	virtual Servlet::BufferedReader& getReader() = 0;
		/// Retrieves the body of the HTTP request from the client to the portal
		/// as character data using a BufferedReader.
		/// The reader translates the character
		/// data according to the character encoding used on the body.
		/// Either this method or {@link #getPortletInputStream} may be called to read the
		/// body, not both.
		/// 
		/// For HTTP POST data of type application/x-www-form-urlencoded
		/// this method throws an IllegalStateException
		/// as this data has been already processed by the 
		/// portal/portlet-container and is available as request parameters.
		///
		/// Returns a BufferedReader containing the body of the request	
		///
		/// Throws UnsupportedEncodingException if the character set encoding 
		///		used is not supported and the text cannot be decoded
		/// Throws IllegalStateException if { getPortletInputStream } method
		///		has been called on this request,it is a HTTP POST data of type 
		///		application/x-www-form-urlencoded.
		/// Throws IOException if an input or output exception occurred
		///    
    
  
	virtual const std::string& getCharacterEncoding() const = 0;
		/// Returns the name of the character encoding used in the body of this request.
		/// This method returns null if the request
		/// does not specify a character encoding.
		///
		/// Returns a string containing the name of the chararacter encoding, or null
		///	if the request does not specify a character encoding.
   

	virtual const std::string& getContentType() const = 0;
		/// Returns the MIME type of the body of the request, 
		/// or null if the type is not known.
		///
		/// Returns a string containing the name of the MIME type of the request, 
		/// or empty string if the type is not known.
   

	virtual int getContentLength() const = 0;
		/// Returns the length, in bytes, of the request body 
		/// which is made available by the input stream, or -1 if the
		/// length is not known. 
		///
		/// Returns an integer containing the length of the 
		/// 			request body or -1 if the length is not known
		///
};


Portlet_END


#endif //Portlet_ActionRequest_INCLUDED
