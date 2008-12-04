/*
 *  ZThreads, a platform-independant, multithreading and 
 *  synchroniation library
 *
 *  Copyright (C) 2000-2002, Eric Crahen, See LGPL.TXT for details
 *
 *  This library is free software; you can redistribute it and/or
 *  modify it under the terms of the GNU Lesser General Public
 *  License as published by the Free Software Foundation; either
 *  version 2.1 of the License, or (at your option) any later version.
 *
 *  This library is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 *  Lesser General Public License for more details.
 *
 *  You should have received a copy of the GNU Lesser General Public
 *  License along with this library; if not, write to the Free Software
 *  Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307 USA
 */

#ifndef __ZTQUEUE_H__
#define __ZTQUEUE_H__

#include "zthread/Cancelable.h"
#include "zthread/NonCopyable.h"

namespace ZThread {

/**
 * @class Queue
 * @author Eric Crahen <crahen@cse.buffalo.edu>
 * @date <2002-06-01T17:37:24-0400>
 * @version 2.2.0
 *
 * A Queue defines an interface of a synchronization point for a collection objects. 
 * It also defines the semantics for how the ownership of these objects is transfered
 * between the Queue and the objects that interact with it.
 *
 * A Queue coordinates the gathering and distribution of items for any set of 
 * threads. 
 */
template <class T>
class Queue : public Cancelable, private NonCopyable {
public:

  //! Destroy a Queue
  virtual ~Queue() throw() { }

  /**
   * Add an object to this Queue, tranfering the ownership of that
   * object to the Queue. As the new owner, the Queue becomes responsible 
   * for deleting the add()ed object. 
   *
   * This method may or may not block the caller for an indefinite amount
   * of time. Those details are defined by specializations of this class.
   *
   * @param item - object to attempt to add to this Queue
   * 
   * @exception Cancellation_Exception thrown if an item is added to a cancel()ed
   * Queue
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception thrown if there is some other error in
   * adding the item to the Queue
   */
  virtual void add(T item)
    /* throw(Synchronization_Exception) */ = 0;

  /**
   * Add an object to this Queue, tranfering the ownership of that
   * object to the Queue. As the new owner, the Queue becomes responsible 
   * for deleting the add()ed object. 
   *
   * This method may or may not block the caller for a definite amount
   * of time. Those details are defined by specializations of this class;
   * however, this method includes a timeout value that can be used to 
   * limit the maximum amount of time that a specialization <i>could</i> block. 
   *
   * @param item - object to attempt to add to this Queue
   * @param timeout - maximum amount of time (milliseconds) this method could block
   * 
   * @return true if the item was add()ed before the given timeout expired. Otherwise
   * false
   *
   * @exception Cancellation_Exception thrown if an item is added to a cancel()ed
   * Queue
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception thrown if there is some other error in
   * adding the item to the Queue
   */
  virtual bool add(T item, unsigned long timeout)
    /* throw(Synchronization_Exception) */ = 0;


  /**
   * Get an object from this Queue, transfering the ownership of that
   * object to the caller. As the new owner, the caller becomes responsible 
   * for deleting the object. 
   *
   * This method may or may not block the caller for an indefinite amount
   * of time. Those details are defined by specializations of this class.
   *
   * @return T next available object
   * 
   * @exception Cancellation_Exception thrown if the Queue has been cancel()ed and
   * no more items are present.
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception thrown if there is some other error in
   * adding the item to the Queue
   */
  virtual T next()
    /* throw(Synchronization_Exception) */ = 0;

  /**
   * Get an object from this Queue, transfering the ownership of that
   * object to the caller. As the new owner, the caller becomes responsible 
   * for deleting the object. 
   *
   * This method may or may not block the caller for a definite amount
   * of time. Those details are defined by specializations of this class;
   * however, this method includes a timeout value that can be used to 
   * limit the maximum amount of time that a specialization <i>could</i> block. 
   *
   * @param timeout - maximum amount of time (millseconds) this method could block
   * @return T next available object
   * 
   * @exception Cancellation_Exception thrown if the Queue has been cancel()ed and
   * no more items are present.
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Timeout_Exception thrown if the given amount
   * of time has expired before an item becomes available
   * @exception Synchronization_Exception thrown if there is some other error in
   * adding the item to the Queue
   */
  virtual T next(unsigned long timeout)
    /* throw(Synchronization_Exception) */ = 0;


  /**
   * Canceling a Queue disables it, disallowing further add()itions. Items already
   * present in the Queue can still be retrieved and are still available through
   * the next() methods.
   *
   * cancel()ing a Queue more than once has no effect.
   * 
   * @post The next() methods will continue to return objects until 
   * the Queue has been emptied. At that point, further calls to the next() methods
   * will throw Cancellation_Exceptions
   *
   * @post The add() methods will throw Cancellation_Exceptions from this point on.
   *
   * @exception Interrupted_Exception should <i>not</i> be thrown. 
   * @exception Synchronization_Exception thrown if there is some error in 
   * cancel()ing the Queue.
   */
  virtual void cancel()
    /* throw(Synchronization_Exception) */ = 0;


  /**
   * Count the items present in this Queue. 
   *
   * This method may or may not block the caller for an indefinite amount
   * of time. Those details are defined by specializations of this class.
   *
   * @return size_t number of elements available in the Queue. These are
   * retrievable through the next() methods.
   * 
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Synchronization_Exception thrown if there is some error in
   * counting the items.
   */
  virtual size_t size()
    /* throw(Synchronization_Exception) */ = 0;

  /**
   * Count the items present in this Queue. 
   *
   * This method may or may not block the caller for an indefinite amount
   * of time. Those details are defined by specializations of this class.
   *
   * This method may or may not block the caller for a definite amount
   * of time. Those details are defined by specializations of this class;
   * however, this method includes a timeout value that can be used to 
   * limit the maximum amount of time that a specialization <i>could</i> block. 
   *
   * @param timeout - maximum amount of time (millseconds) this method could block

   * @return size_t number of elements available in the Queue. These are
   * retrievable through the next() methods.
   * 
   * @exception Interrupted_Exception thrown if the the method is invoked from the 
   * context of a thread that has been interrupt()ed.
   * @exception Timeout_Exception thrown if the given amount
   * of time has expired before an item becomes available
   * @exception Synchronization_Exception thrown if there is some error in
   * counting the items.
   */
  virtual size_t size(unsigned long timeout)
    /* throw(Synchronization_Exception) */ = 0;


  /**
   * Test this Queue to see if it is empty by testing the size of the 
   * Queue.
   *
   * @return boolean  true if empty, otherwise false
   *
   * @exception Cancellation_Exception should not be thrown. A cancel()ed
   * Queue that contains no more elements should report that it is 
   * empty.
   *
   * @see Queue::size()
   */
  virtual bool empty()
    /* throw(Synchronization_Exception) */ {

    try {

      return size() == 0;

    } catch(Cancellation_Exception&) { }

    return true;

  }

  /**
   * Test this Queue to see if it is empty by testing the size of the 
   * Queue.
   *
   * @param timeout - maximum amount of time (millseconds) this method could block
   *
   * @return boolean  true if empty, otherwise false
   *
   * @exception Cancellation_Exception should not be thrown. A cancel()ed
   * Queue that contains no more elements should report that it is 
   * empty.
   *
   * @see Queue::size(unsigned long)
   */
  virtual bool empty(unsigned long timeout)
    /* throw(Synchronization_Exception) */ {

    try {

      return size(timeout) == 0;

    } catch(Cancellation_Exception&) { }

    return true;

  }

};

} // namespace ZThread

#endif // __ZTQUEUE_H__
