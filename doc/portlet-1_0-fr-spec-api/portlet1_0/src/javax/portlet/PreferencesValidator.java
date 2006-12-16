/**
  * Copyright 2003 IBM Corporation and Sun Microsystems, Inc.
  * All rights reserved.
  * Use is subject to license terms.
  */

package javax.portlet;


/**
 * The <CODE>PreferencesValidator</CODE> allows to validate the set of
 * preferences of the associated portlet just before they are
 * stored in the persistent store.
 * <p>
 * The portlet container invokes the <code>validate</code> method as 
 * part of the invocation of the <code>store</code> method of the
 * <code>PortletPreferences</code>.
 */
public interface PreferencesValidator
{

  /**
   * If the preferences values are successfully validated the call to this method
   * must finish gracefully. Otherwise it must throw a <code>ValidatorException</code>.
   *
   * @param  preferences   preferences to validate
   *
   * @throws  ValidatorException  if the given preferences contains invalid
   *                              settings
   *
   */

  public void validate(PortletPreferences preferences)
    throws ValidatorException;  
}
