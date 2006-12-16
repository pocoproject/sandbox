/**
  * Copyright 2003 IBM Corporation and Sun Microsystems, Inc.
  * All rights reserved.
  * Use is subject to license terms.
  */

package javax.portlet;



/**
 * The <code>PortletRequestDispatcher</code> interface 
 * defines an object that receives requests from the client
 * and sends them to the specified resources (such as a servlet,
 * HTML file, or JSP file) on the server. The portlet
 * container creates the <code>PortletRequestDispatcher</code> object,
 * which is used as a wrapper around a server resource located
 * at a particular path or given by a particular name.
 *
 */
 
public interface PortletRequestDispatcher {

    /**
     *
     * Includes the content of a resource (servlet, JSP page,
     * HTML file) in the response. In essence, this method enables 
     * programmatic server-side includes.
     * <p>
     * The included servlet cannot set or change the response status code
     * or set headers; any attempt to make a change is ignored.
     *
     *
     * @param request 			a {@link RenderRequest} object 
     *					that contains the client request
     *
     * @param response 			a {@link RenderResponse} object 
     * 					that contains the render response
     *
     * @exception PortletException 	if the included resource throws a ServletException, 
     *                                  or other exceptions that are not Runtime-
     *                                  or IOExceptions.
     *
     * @exception java.io.IOException	if the included resource throws this exception
     *
     *
     */
     
    public void include(RenderRequest request, RenderResponse response)
	throws PortletException, java.io.IOException;



}








