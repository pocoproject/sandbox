/**
  * Copyright 2003 IBM Corporation and Sun Microsystems, Inc.
  * All rights reserved.
  * Use is subject to license terms.
  */

package javax.portlet;

/**
 ** The <CODE>WindowStateException</CODE> is thrown when a portlet
 ** tries to use a window state that is not supported by the current
 ** runtime environment or the portlet.
 **/

public class WindowStateException extends PortletException
{

  private transient WindowState _state = null;

  /**
   * Constructs a new portlet state exception with the given text. The
   * portlet container may use the text write it to a log.
   *
   * @param   text
   *          the exception text
   * @param   state
   *          the state causing the exception
   */

  public WindowStateException (String text, WindowState state)
  {
    super (text);
    _state = state;
  }

  /**
   * Constructs a new portlet state exception when the portlet needs to do
   * the following:
   * <ul>
   * <il>throw an exception 
   * <li>include a message about the "root cause" that interfered
   *     with its normal operation
   * <li>include a description message
   * </ul>
   *
   * @param   text
   *          the exception text
   * @param   cause
   *          the root cause
   * @param   state
   *          the state causing the exception
   */
  
  public WindowStateException (String text, Throwable cause, WindowState state)
  {
    super(text, cause);
    _state = state;
  }

  /**
   * Constructs a new portlet state exception when the portlet needs to throw an
   * exception. The exception message is based on the localized message
   * of the underlying exception.
   *
   * @param   cause
   *          the root cause
   * @param   state
   *          the state causing the exception
   */

  public WindowStateException (Throwable cause, WindowState state)
  {
    super(cause);
    _state = state;
  }

  /**
   * Returns the portlet state causing this exception.
   * 
   * @return  the window state causing this exception
   */

  public WindowState getState()
  {
    return _state;
  }
}
