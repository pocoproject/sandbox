/// RenderResponse.cpp

/// Copyright 2003 IBM Corporation and Sun Microsystems, Inc.

#ifndef Portlet_RenderResponse_INCLUDED
#define Portlet_RenderResponse_INCLUDED


#ifndef Portlet_PortletBase_INCLUDED
#include "Portlet/PortletBase.h"
#endif
/*
#ifndef Portlet_PortletURL_INCLUDED
#include "Portlet/PortletURL.h"
#endif
*/
#ifndef Portlet_PortletResponse_INCLUDED
#include "Portlet/PortletResponse.h"
#endif
#ifndef Servlet_PrintWriter_INCLUDED
#include "Servlet/PrintWriter.h"
#endif
#ifndef Servlet_ServletOutputStream_INCLUDED
#include "Servlet/ServletOutputStream.h"
#endif


Portlet_BEGIN

class PortletURL;

class CPP_PORTLET_DLL_API RenderResponse : public PortletResponse
	/// The RenderResponse defines an object to assist a portlet in
	/// sending a response to the portal.
	/// It extends the PortletResponse interface to provide specific 
	/// render response functionality to portlets.
	/// The portlet container creates a RenderResponse object and 
	/// passes it as argument to the portlet's render method.
{
public:
	typedef Servlet::ServletOutputStream OutputStream;

	static const std::string EXPIRATION_CACHE;
  		/// Property to set the expiration time in seconds for this
  		/// response using the setProperty method. 
  		/// 
  		/// If the expiration value is set to 0, caching is disabled 
  		/// for this portlet; if the value is set to -1, 
  		/// the cache does not expire.
  		/// 
  		/// The value is "portlet.expiration-cache".

	virtual const std::string& getContentType() const = 0;
  		/// Returns the MIME type that can be used to contribute
  		/// markup to the render response.
  		/// 
  		/// If no content type was set previously using the { setContentType } method
  		/// this method retuns null.

	virtual const PortletURL& createRenderURL() = 0;
  		/// Creates a portlet URL targeting the portlet. If no portlet mode, 
  		/// window state or security modifier is set in the PortletURL the
  		/// current values are preserved. If a request is triggered by the
  		/// PortletURL, it results in a render request.
  		/// 
  		/// The returned URL can be further extended by adding
  		/// portlet-specific parameters and portlet modes and window states. 
  		/// 
  		/// The created URL will per default not contain any parameters
  		/// of the current render request.
  
	virtual const PortletURL& createActionURL() = 0;
  		/// Creates a portlet URL targeting the portlet. If no portlet mode, 
  		/// window state or security modifier is set in the PortletURL the
  		/// current values are preserved. If a request is triggered by the
  		/// PortletURL, it results in an action request.
  		/// 
  		/// The returned URL can be further extended by adding
  		/// portlet-specific parameters and portlet modes and window states. 
  		/// 
  		/// The created URL will per default not contain any parameters
  		/// of the current render request.

	virtual const std::string& getNamespace() const = 0;
  		/// The value returned by this method should be prefixed or appended to 
  		/// elements, such as JavaScript variables or function names, to ensure 
  		/// they are unique in the context of the portal page.

	virtual void setTitle(const std::string& title) = 0;
  		/// This method sets the title of the portlet.

	virtual void setContentType(const std::string& type) = 0;
  		/// Sets the MIME type for the render response. The portlet must
  		/// set the content type before calling {getWriter} or
  		/// {getPortletOutputStream}.
  		/// 
  		/// Calling setContentType after getWriter
  		/// or getOutputStream does not change the content type.
  		///
  		/// Parameter type is the content MIME type
  		///
  		/// Throws IllegalArgumentException if the given type is not 
		/// in the list returned by PortletRequest.getResponseContentTypes

	virtual const std::string& getCharacterEncoding() const = 0;
  		/// Returns the name of the charset used for
  		/// the MIME body sent in this response.
  		///
  		/// See <a href="http://ds.internic.net/rfc/rfc2045.txt">RFC 2047</a>
  		/// for more information about character encoding and MIME.
  		///
  		/// Returns a String specifying the name of the charset,
		///	for example: ISO-8859-1

	virtual Servlet::PrintWriter& getWriter() = 0;
  		/// Returns a PrintWriter object that can send character 
  		/// text to the portal.
  		/// 
  		/// Before calling this method the content type of the
  		/// render response must be set using the {@link #setContentType}
  		/// method.
  		/// 
  		/// Either this method or {@link #getPortletOutputStream} may be 
  		/// called to write the body, not both.
  		///
  		/// Throws IOException if an input or output exception occurred
  		/// Throws IllegalStateException if the getPortletOutputStream method
  		/// has been called on this response, or if no content type was set 
		/// using the setContentType method.

	virtual const std::locale& getLocale() const = 0;
  		/// Returns the locale assigned to the response.
  
	virtual void setBufferSize(int size) = 0;
  		/// Sets the preferred buffer size for the body of the response.  
  		/// The portlet container will use a buffer at least as large as 
  		/// the size requested.
  		/// 
  		/// This method must be called before any response body content is
  		/// written; if content has been written, or the portlet container
  		/// does not support buffering, this method may throw an 
  		/// IllegalStateException.
  		///
  		/// Parameter size 	the preferred buffer size
  		///
  		/// Throws IllegalStateException if this method is called after
  		///	content has been written, or the portlet container does not 
		/// support buffering.

	virtual int getBufferSize() const = 0;
  		/// Returns the actual buffer size used for the response.  If no buffering
  		/// is used, this method returns 0    

	virtual void flushBuffer() = 0;
  		/// Forces any content in the buffer to be written to the client.  A call
  		/// to this method automatically commits the response.
  		///
  		/// Throws IOException  if an error occured when writing the output.

	virtual void resetBuffer() = 0;
  		/// Clears the content of the underlying buffer in the response without
  		/// clearing properties set. If the response has been committed, 
  		/// this method throws an IllegalStateException.
  		///
  		/// throws IllegalStateException if this method is called after
  		///	response is comitted.  

	virtual bool isCommitted() const = 0;
  		/// Returns a boolean indicating if the response has been
  		/// committed.  

	virtual void reset() = 0;
  		/// Clears any data that exists in the buffer as well as the properties set.
  		/// If the response has been committed, this method throws an 
  		/// IllegalStateException.
  		///
  		/// Throws IllegalStateException if the response has already been committed    

	virtual OutputStream& getPortletOutputStream() = 0;
  		/// Returns a OutputStream suitable for writing binary 
  		/// data in the response. The portlet container does not encode the
  		/// binary data.  
  		/// 
  		/// Before calling this method the content type of the
  		/// render response must be set using the {@link #setContentType}
  		/// method.
  		/// 
  		/// Calling flush() on the OutputStream commits the response.
  		/// 
  		/// Either this method or {@link #getWriter} may be called to write the body, not both.
  		///
  		/// Throws IllegalStateException if the getWriter method
  		/// has been called on this response, or if no content type was set using the
  		/// setContentType method.
  		///
  		/// Throws IOException 	if an input or output exception occurred.
};


Portlet_END


#endif //Portlet_RenderResponse_INCLUDED
