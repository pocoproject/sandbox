//
// FIFOBuffer.h
//
// $Id: //poco/1.4/Foundation/include/Poco/FIFOBuffer.h#2 $
//
// Library: Foundation
// Package: Core
// Module:  FIFOBuffer
//
// Definition of the FIFOBuffer class.
//
// Copyright (c) 2006, Applied Informatics Software Engineering GmbH.
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


#ifndef Foundation_FIFOBuffer_INCLUDED
#define Foundation_FIFOBuffer_INCLUDED


#include "Poco/Foundation.h"
#include "Poco/Exception.h"
#include "Poco/Buffer.h"
#include "Poco/BasicEvent.h"
#include "Poco/Mutex.h"
#include "Poco/Format.h"


namespace Poco {


template <class T>
class BasicFIFOBuffer
	/// A simple buffer class with support for re-entrant,
	/// FIFO-style read/write operations, as well as (optional)
	/// empty/non-empty/full (i.e. writable/readable) transition
	/// notifications. Buffer size, as well as amount of unread data
	/// and available space introspections are supported as well.
	///
	/// This class is useful anywhere where a FIFO functionality
	/// is needed.
{
public:
	typedef T Type;

	mutable Poco::BasicEvent<bool> writable;
		/// Event indicating "writability" of the buffer,
		/// triggerred as follows:
		///
		///	* when buffer transitions from non-full to full, 
		///	  Writable event observers are notified, with 
		///	  false value as the argument
		///
		///	* when buffer transitions from full to non-full,
		///	  Writable event observers are notified, with 
		///	  true value as the argument

	mutable Poco::BasicEvent<bool> readable;
		/// Event indicating "readability" of the buffer,
		/// triggerred as follows:
		///
		///	* when buffer transitions from non-empty to empty,
		///	  Readable event observers are notified, with false  
		///	  value as the argument
		///
		///	* when FIFOBuffer transitions from empty to non-empty,
		///	  Readable event observers are notified, with true value
		///	  as the argument

	BasicFIFOBuffer(std::size_t size, bool notify = false):
		_buffer(size),
		_begin(0),
		_used(0),
		_notify(notify)
		/// Creates and allocates the FIFOBuffer.
	{
	}

	~BasicFIFOBuffer()
		/// Destroys the FIFOBuffer.
	{
	}
	
	void resize(std::size_t newSize, bool preserveContent = true)
		/// Resizes the buffer. If preserveContent is true,
		/// the content of the old buffer is preserved.
		/// New size can be larger or smaller than
		/// the current size, but it must not be 0.
		/// Additionally, if the new length is smaller
		/// than currently used length and preserveContent
		/// is true, InvalidAccessException is thrown.
	{
		Mutex::ScopedLock lock(_mutex);

		if (preserveContent && (newSize < _used))
			throw InvalidAccessException("Can not resize FIFO without data loss.");
		
		std::size_t usedBefore = _used;
		_buffer.resize(newSize, preserveContent);
		if (!preserveContent) _used = 0;
		if (_notify) notify(usedBefore);
	}
	
	std::size_t peek(Poco::Buffer<T>& buffer, std::size_t length = 0) const
		/// Peeks into the data currently in the FIFO
		/// without actually extracting it.
		/// Resizes the supplied buffer to the size of
		/// data written to it. If length is not
		/// supplied by the caller, the current FIFO used length
		/// is substituted for it.
		/// 
		/// Returns the reference to the buffer.
	{
		Mutex::ScopedLock lock(_mutex);

		if (0 == length || length > _used) length = _used;
		poco_assert (length <= _buffer.size());
		buffer.resize(length);
		std::memcpy(buffer.begin(), _buffer.begin() + _begin, length * sizeof(T));

		return length;
	}
	
	std::size_t read(Poco::Buffer<T>& buffer, std::size_t length = 0)
		/// Copies the data currently in the FIFO
		/// into the supplied buffer.
		/// Resizes the supplied buffer to the size of
		/// data written to it.
		/// 
		/// Returns the reference to the buffer.
	{
		Mutex::ScopedLock lock(_mutex);

		if (0 == _used) return 0;

		std::size_t usedBefore = _used;
		std::size_t readLen = peek(buffer, length);
		poco_assert (_used >= readLen);
		_used -= readLen;
		if (0 == _used) _begin = 0;
		else _begin += length;

		if (_notify) notify(usedBefore);

		return readLen;
	}

	std::size_t write(const Buffer<T>& buffer, std::size_t length = 0)
		/// Writes data from supplied buffer to the FIFO buffer.
		/// If there is no sufficient space for the whole
		/// buffer to be written, data up to available 
		/// length is written.
		/// The length of data to be written is determined from the
		/// length argument or buffer size (when length argument is
		/// default zero or greater than buffer size).
		/// 
		/// Returns the length of data written.
	{
		Mutex::ScopedLock lock(_mutex);

		if (0 == length || length > buffer.size()) length = buffer.size();

		if (_buffer.size() - (_begin + _used) < length)
		{
			std::memmove(_buffer.begin(), _buffer.begin() + _begin, _used);
			_begin = 0;
		}

		std::size_t usedBefore = _used;
		std::size_t available =  _buffer.size() - _used - _begin;
		std::size_t len = length > available ? available : length;
		std::memcpy(_buffer.begin() + _begin + _used, buffer.begin(), len * sizeof(T));
		_used += len;
		poco_assert (_used <= _buffer.size());
		if (_notify) notify(usedBefore);

		return len;
	}

	std::size_t size() const
		/// Returns the size of the buffer.
	{
		return _buffer.size();
	}
	
	std::size_t used() const
		/// Returns the size of the used portion of the buffer.
	{
		return _used;
	}
	
	std::size_t available() const
		/// Returns the size of the available portion of the buffer.
	{
		return _buffer.size() - _used;
	}

	void drain(std::size_t length = 0)
		/// Drains length number of elements from the buffer.
		/// If length is zero or greater than buffer current
		/// content length, buffer is emptied.
	{
		std::size_t usedBefore = _used;

		if (0 == length || length >= _used)
		{
			_begin = 0;
			_used = 0;
		}
		else
			_used -= length;

		if (_notify) notify(usedBefore);
	}

	void copy(const T* ptr, std::size_t length)
		/// Copies the supplied data to the buffer and adjusts
		/// the used buffer size.
	{
		poco_check_ptr(ptr);
		if (0 == length) return;
		if (length > available())
			throw Poco::InvalidAccessException("Cannot extend buffer.");

		std::memcpy(&_buffer[_used], ptr, length);
		advance(length);
	}

	void advance(std::size_t length)
		/// Advances buffer by length elements.
		/// Should be called AFTER the data 
		/// was copied into the buffer.
	{
		if (length > available())
			throw Poco::InvalidAccessException("Cannot extend buffer.");

		std::size_t usedBefore = _used;
		_used += length;
		if (_notify) notify(usedBefore);
	}

	T* begin()
		/// Returns the pointer to the beginning of the buffer.
	{
		return _buffer.begin();
	}

	T* next()
		/// Returns the pointer to the next available position in the buffer.
	{
		if (available() == 0)
			throw InvalidAccessException("Buffer is full.");

		return _buffer.begin() + _used;
	}

	T& operator [] (std::size_t index)
		/// Returns value at index position.
		/// Throws InvalidAccessException if index is larger than 
		/// the last valid (used) buffer position.
	{
		if (index >= _used)
			throw InvalidAccessException(format("Index out of bounds: %z (max index allowed: %z)", index, _used - 1));

		return _buffer[_begin + index];
	}

	const T& operator [] (std::size_t index) const
		/// Returns value at index position.
		/// Throws InvalidAccessException if index is larger than 
		/// the last valid (used) buffer position.
	{
		if (index >= _used)
			throw InvalidAccessException(format("Index out of bounds: %z (max index allowed: %z)", index, _used - 1));

		return _buffer[_begin + index];
	}

	const Buffer<T>& buffer() const
		/// Returns const reference to the underlying buffer.
	{
		return _buffer;
	}

	bool isEmpty() const
		/// Returns true is buffer is empty, flase otherwise.
	{
		return 0 == _used;
	}

private:
	void notify(std::size_t usedBefore)
	{
		bool t = true, f = false;
		if (usedBefore == 0 && _used > 0)
			readable.notify(this, t);
		else if (usedBefore > 0 && 0 == _used)
			readable.notify(this, f);
		
		if (usedBefore == _buffer.size() && _used < _buffer.size())
			writable.notify(this, t);
		else if (usedBefore < _buffer.size() && _used == _buffer.size())
			writable.notify(this, f);
	}

	BasicFIFOBuffer();
	BasicFIFOBuffer(const BasicFIFOBuffer&);
	BasicFIFOBuffer& operator = (const BasicFIFOBuffer&);

	Buffer<T>     _buffer;
	std::size_t   _begin;
	std::size_t   _used;
	bool          _notify;
	mutable Mutex _mutex;
};


//
// We provide an instantiation for char
//
typedef BasicFIFOBuffer<char> FIFOBuffer;


} // namespace Poco


#endif // Foundation_FIFOBuffer_INCLUDED
