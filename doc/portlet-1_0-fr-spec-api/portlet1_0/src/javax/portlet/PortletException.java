/**
  * Copyright 2003 IBM Corporation and Sun Microsystems, Inc.
  * All rights reserved.
  * Use is subject to license terms.
  */

package javax.portlet;



/**
 * The <CODE>PortletException</CODE> class defines a general exception
 * that a portlet can throw when it is unable to perform its operation
 * successfully.
 */

public class PortletException extends java.lang.Exception
{

  private Throwable _cause;


  /**
   * Constructs a new portlet exception.
   */

  public PortletException ()
  {
    super();
  }

  /**
   * Constructs a new portlet exception with the given text. The
   * portlet container may use the text write it to a log.
   *
   * @param   text
   *          the exception text
   */

  public PortletException (String text)
  {
    super (text);
  }

  /**
   * Constructs a new portlet exception when the portlet needs to do
   * the following:
   * <ul>
   * <li>throw an exception 
   * <li>include the "root cause" exception
   * <li>include a description message
   * </ul>
   *
   * @param   text
   *          the exception text
   * @param   cause
   *          the root cause
   */
  
  public PortletException (String text, Throwable cause)
  {
    super(text);
    _cause = cause;
    // change this when going to jdk1.4:    super (text, cause);
  }

  /**
   * Constructs a new portlet exception when the portlet needs to throw an
   * exception. The exception's message is based on the localized message
   * of the underlying exception.
   *
   * @param   cause
   *          the root cause
   */

  public PortletException (Throwable cause)
  {
    _cause = cause;
    // change this when going to jdk1.4:        super (cause);
  }

  /**
   * Prints the stack trace of this exception to the standard error stream.
   */
  public void printStackTrace()
  {
    this.printStackTrace(System.err);
  }
  
  /**
   * Prints the stack trace of this exception to the specified print stream.
   *
   * @param out the <code>PrintStream</code> to be used for output
   */
  public void printStackTrace(java.io.PrintStream out) 
  {
    this.printStackTrace(new java.io.PrintWriter(out, true));
  }

  /**
   * Prints the stack trace of this exception to the specified print writer.
   * 
   * @param out the <code>PrintWriter</code> to be used for output
   */
  public void printStackTrace(java.io.PrintWriter out)
  {
    super.printStackTrace(out);

    if( getCause () != null ) {
      out.println();
      out.print("Nested Exception is ");
      getCause ().printStackTrace(out);
    }
    // change this when going tojdk1.4:
      /*
        super.printStackTrace(out);

        if( getRootCause () != null )
        {
            out.println();
            out.print("Nested Exception is ");
            getRootCause ().printStackTrace(out);
        }
        */
  }

  /**
   * Returns the cause of this throwable or <code>null</code> if the
   * cause is nonexistent or unknown.  (The cause is the throwable that
   * caused this throwable to get thrown.)
   *
   * <p>This implementation returns the cause that was supplied via one of
   * the constructors requiring a <tt>Throwable</tt>.
   *
   * @return  the cause of this throwable or <code>null</code> if the
   *          cause is nonexistent or unknown.
   */
  public Throwable getCause() {
    return (_cause!=null ? _cause : null);
  }

}
