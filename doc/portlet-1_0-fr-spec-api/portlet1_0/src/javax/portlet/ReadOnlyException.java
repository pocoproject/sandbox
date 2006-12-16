/**
  * Copyright 2003 IBM Corporation and Sun Microsystems, Inc.
  * All rights reserved.
  * Use is subject to license terms.
  */

package javax.portlet;

/**
 * The <CODE>ReadOnlyException</CODE> is thrown when a portlet tries
 * to change the value for a read-only preference attribute.
 */

public class ReadOnlyException extends PortletException
{

  private ReadOnlyException ()
  {
  }

  /**
   * Constructs a new read-only exception with the given text. The
   * portlet container may use the text write it to a log.
   *
   * @param   text
   *          the exception text
   */

  public ReadOnlyException (String text)
  {
    super (text);
  }

  /**
   * Constructs a new read-only exception when the portlet needs to do
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
   */
  
  public ReadOnlyException (String text, Throwable cause)
  {
    super(text, cause);
  }

  /**
   * Constructs a new read-only exception when the portlet needs to throw an
   * exception. The exception message is based on the localized message
   * of the underlying exception.
   *
   * @param   cause
   *          the root cause
   */

  public ReadOnlyException (Throwable cause)
  {
    super(cause);
  }


}
