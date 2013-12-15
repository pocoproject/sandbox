// vi:set ts=4 sw=4 nowrap:

#ifndef POCO_DELIMITER_STREAM_H
#define POCO_DELIMITER_STREAM_H

// Copyright 2013 Dieter Lucking
// Copyright 2007 Dieter Lucking
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

// Note:
// Changes applied by Dieter Lucking.

/// \file
/// Streams for delimited input/output.

#include <algorithm>
#include <cstring>
#include <istream>
#include <ostream>

namespace Poco {

// Forward
// ============================================================================

template<typename Char, typename Traits> class BasicDelimiterInputBuffer;
template<typename Char, typename Traits> class BasicDelimiterOutputBuffer;


namespace Detail {

// DelimiterCharacterClassification
// ============================================================================

/// A ctype implementation for streams holding values separated by a distinct
/// character. Only the field and record separator are space characters.
template <class Char>
class DelimiterCharacterClassification : public std::ctype< Char >
{
	private:
	typedef typename std::ctype<Char> Base;

	public:
	typedef typename Base::mask mask;
	typedef typename Base::char_type char_type;

	DelimiterCharacterClassification(const Base& forward);

	const std::ctype<Char>& forward() const { return m_forward; }
	/// The field separator '\x1F'.
	Char field_separator() const { return m_field_separator; }
	/// The record separator '\x1E'.
	Char record_separator() const { return m_record_separator; }


	protected:
	bool do_is(mask m, Char) const;
	const Char* do_is(const Char* low, const Char* high, mask* result) const;
	const Char* do_scan_is(mask m, const Char* low, const Char* high) const;
	const Char* do_scan_not(mask m, const Char* low, const Char* high) const;

	private:
	const Base& m_forward;
	Char m_field_separator;
	Char m_record_separator;
};

template <class Char>
DelimiterCharacterClassification<Char>::DelimiterCharacterClassification(
	const Base& forward)
:   m_forward(forward),
	m_field_separator(m_forward.widen(Char(0x1F))),
	m_record_separator(m_forward.widen(Char(0x1E)))
{}


template <class Char>
bool DelimiterCharacterClassification<Char>::do_is(mask m, Char c) const {
	if(m & std::ctype_base::space)
		return (c == field_separator() || c == record_separator());
	else {
		m &= ~std::ctype_base::space;
		if( ! m) return false;
		else return m_forward.is(m, c);
	}
}

template <class Char>
const Char* DelimiterCharacterClassification<Char>::do_is(
	const Char* low, const Char* high, mask* result) const
{
	m_forward.is(low, high, result);
	while(low < high) {
		*result &= ~std::ctype_base::space;
		if(*low == field_separator() || *low == record_separator())
			*result |= std::ctype_base::space;
		++low;
		++result;
	}
	return high;
}

template <class Char>
const Char* DelimiterCharacterClassification<Char>::do_scan_is(
	mask m, const Char* low, const Char* high) const
{
	const Char* sp = high;
	if(m & std::ctype_base::space) {
		for(sp = low; sp < high; ++sp) {
			if(*sp == field_separator() || *sp == record_separator())
				break;
		}
	}
	m &= ~std::ctype_base::space;
	if( ! m) return high;
	else return m_forward.scan_is(m, low, sp);
}

template <class Char>
const Char* DelimiterCharacterClassification<Char>::do_scan_not(
	mask m, const Char* low, const Char* high) const
{
	const Char* sp = high;
	if(m & std::ctype_base::space) {
		for(sp = low; sp < high; ++sp) {
			if(*sp != field_separator() && *sp != record_separator())
				break;
		}
	}
	m &= ~std::ctype_base::space;
	if( ! m) return sp;
	else return m_forward.scan_not(m, low, sp);
}


/// The char implementation of DelimiterCharacterClassification.
template<>
class DelimiterCharacterClassification<char> : public std::ctype<char>
{
	private:
	typedef std::ctype< char > Base;
	const mask* initialize_table(const Base&);

	public:
	typedef Base::mask mask;
	typedef Base::char_type char_type;

	public:
	/// Initialize.
	DelimiterCharacterClassification(const Base& forward)
	:   Base(initialize_table(forward)),
		m_forward(forward)
	{}

	const std::ctype<char>& forward() const { return m_forward; }

	/// The field separator'\x1F'.
	char field_separator() const { return char(0x1F); }
	/// The record separator '\x1E'.
	char record_separator() const { return char(0x1E); }

	private:
	mask m_table[Base::table_size];
	const Base& m_forward;
};

inline const typename DelimiterCharacterClassification< char>::mask*
DelimiterCharacterClassification<char>::initialize_table(const Base& formard) {
	const mask* src = formard.table();
	const mask* src_end = src + Base::table_size;
	mask* dst = m_table;
	for( ; src < src_end; ++dst, ++src) {
		*dst = *src & ~std::ctype_base::space;
	}
	m_table[(unsigned char)field_separator()] |= std::ctype_base::space;
	m_table[(unsigned char)record_separator()] |= std::ctype_base::space;
	return m_table;
}

// Logging
// ============================================================================

template<typename Char, typename Traits>
void log_delimiter_stream(
	const BasicDelimiterInputBuffer<Char, Traits>&,
	int line);

template<typename Char, typename Traits>
void log_delimiter_stream(
	const BasicDelimiterOutputBuffer<Char, Traits>&,
	int line);


} // namespace Detail


// BasicDelimiterInputBuffer
// ============================================================================

/// A stream buffer for streams holding values separated by particular characters.
template <typename Char, typename Traits = std::char_traits<Char> >
class BasicDelimiterInputBuffer : public std::basic_streambuf<Char, Traits>
{
	// Types
	// =====

    private:
	typedef std::basic_streambuf<Char, Traits> Base;

    public:
	typedef typename Base::char_type char_type;
	typedef typename Base::int_type int_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::off_type off_type;
	typedef typename Base::traits_type traits_type;

	typedef typename std::basic_string<char_type> string_type;

	// Construction
	// ============

    public:
	/// Connect to the given input stream.
	BasicDelimiterInputBuffer(
		std::basic_ios<Char, Traits>& delimiter_stream,
		std::basic_istream<Char, Traits>&,
		char_type delimiter = 0,
		char_type quote = 0,
		char_type carriage_return = 0,
		char_type newline = 0);


	// Element Access
	// ==============

    public:
	std::basic_istream<Char, Traits>& istream() const { return *m_is; }
	char_type delimiter() const	{ return m_delimiter; }
	char_type quote() const { return m_quote; }
	char_type carriage_return() const { return m_carriage_return; }
	char_type newline() const { return m_newline; }
	char_type field_separator() const	{ return m_dtype->field_separator(); }
	char_type record_separator() const	{ return m_dtype->record_separator(); }

	/// The empty value [Initial: Two Quotes].
	const string_type& empty_value() const { return m_empty_value; }

	/// Set the empty value.
	string_type empty_value(const string_type& value) {
		string_type result = m_empty_value;
		m_empty_value = value;
		return result;
	}

	/// Set the empty value.
	string_type empty_value(char_type value) { return empty_value(string_type(1, value)); }

	/// The extracted separator.
	char_type separator() const { return m_separator; }

	/// The extracted field.
	const string_type& field() const { return m_field; }

	/// \True if extracted field is replaced by the empty value.
	bool empty() const { return m_empty; }

	/// Skip a separator, if it is the next available character.
	bool skip_separator() {
		if((this->egptr() - this->gptr()) == 1
		&& (*this->gptr() == field_separator() || *this->gptr() == record_separator())) {
			clear();
			return true;
		}
		return false;
	}

	/// Skip a field and return one if a field is skipped, zero otherwise.
	unsigned skip_field() {
		skip_separator();
		if(is_eof(underflow())) return 0;
		else {
			clear();
			return 1;
		}
	}

	/// Skip the next n fields and return the number of fields skipped.
	unsigned skip_fields(unsigned n) {
		unsigned i = 0;
		for( ; i < n; ++i) {
			skip_separator();
			if(is_eof(underflow())) break;
			else clear();
		}
		return i;
	}

	/// Skip the rest of a line and return the number of fields skipped.
	unsigned skip_line() {
		skip_separator();
		unsigned n = 0;
		while( ! is_eof(underflow())) {
			++n;
			bool record = (m_separator == record_separator());
			clear();
			if(record) break;
		}
		return n;
	}

	/// Match the next character with the prefix character and skip that
	/// character and the rest of a line. The function returns the number of
	/// fields skipped.
	unsigned skip_line(char_type prefix) {
		skip_separator();
		int_type ch = underflow();
		if(traits_type::to_int_type(prefix) == ch) {
			return skip_line();
		}
		return 0;
	}

    // Stream Buffer Interface
	// =======================

    protected:
	virtual std::streamsize showmanyc();
	virtual std::streamsize xsgetn(char_type*, std::streamsize);
	virtual int_type underflow();
	virtual int_type pbackfail(int_type);

    // Utilities
	// =========

    protected:
	int_type eof() { return traits_type::eof(); }

	int_type get() { return m_is->rdbuf()->sbumpc(); }
	int_type peek() { return m_is->rdbuf()->sgetc(); }

	void append(char_type c) { m_field += c; }
	void append(int_type ch) { append(traits_type::to_char_type(ch)); }

	void clear() {
		m_field.clear();
		this->setg(0, 0, 0);
	}

	void update_get_area() {
		if(m_field.empty()) this->setg(0, 0, 0);
		else {
			char_type* p = &m_field[0];
			this->setg(p, p, p + m_field.size());
		}
	}

	bool is_eof(int_type ch) { return ch == eof(); }
	bool is_space(char_type c) { return m_dtype->forward().is(std::ctype_base::space, c); }
	bool is_space(int_type ch) { return is_space(traits_type::to_char_type(ch)); }
	bool is_nl(char_type c) { return c == newline(); }
	bool is_nl(int_type ch) { return is_nl(traits_type::to_char_type(ch)); }

    private:
	std::basic_ios<Char, Traits>& m_ds;
	std::basic_istream<Char, Traits>* m_is;
	Detail::DelimiterCharacterClassification<Char>* m_dtype;
	char_type m_delimiter;
	char_type m_quote;
	char_type m_carriage_return;
	char_type m_newline;
	char_type m_separator;
	string_type m_empty_value;
	string_type m_field;
	bool m_empty;
};


template < typename Char, typename Traits>
BasicDelimiterInputBuffer<Char, Traits>::BasicDelimiterInputBuffer(
	std::basic_ios<Char, Traits>& delimiter_stream,
	std::basic_istream<Char, Traits>& is,
	char_type delimiter,
	char_type quote,
	char_type carriage_return,
	char_type newline)
:   m_ds(delimiter_stream),
	m_is(&is),
    m_dtype(new Detail::DelimiterCharacterClassification<Char>(
    	std::use_facet<std::ctype<Char> >(is.getloc()))),
	m_delimiter(delimiter ? delimiter : m_dtype->widen(',')),
	m_quote(quote ? quote : m_dtype->widen('"')),
	m_carriage_return(carriage_return ? carriage_return : m_dtype->widen('\r')),
	m_newline(newline ? newline : m_dtype->widen('\n')),
    m_separator(0),
    m_empty_value(2, m_quote),
    m_empty(true)
{
	delimiter_stream.imbue(std::locale(m_is->getloc(), m_dtype));
}

// Logging
// =======

namespace Detail {
	template<typename Char, typename Traits>
	inline void log_delimiter_stream(
		const BasicDelimiterInputBuffer<Char, Traits>&,
		int line)
	{}

	#if POCO_LOG_DELIMITER_INPUT_BUFFER_ENABLE
	template<typename Traits>
	inline void log_delimiter_stream(
		const BasicDelimiterInputBuffer<char, Traits>& sb,
		int line)
	{
		std::clog
			<< line << "  Input:  "
			<< ((sb.empty()) ? 'e' : '-')
			<< ((sb.separator())
			       ? ((sb.separator() == sb.field_separator()) ? 'f' : 'r')
			       : '-')
			<< ' '
			<< sb.field()
			<< std::endl;
	}
	#endif
}

// Get Area
// ========

template < typename Char, typename Traits>
std::streamsize BasicDelimiterInputBuffer<Char, Traits>::showmanyc() {
	return m_field.size();
}

template < typename Char, typename Traits>
std::streamsize
BasicDelimiterInputBuffer<Char, Traits>::xsgetn(char_type* p, std::streamsize n) {
	std::streamsize result = 0;
	while(result < n && ! is_eof(underflow())) {
		std::streamsize k = std::min(n - result, this->egptr() - this->gptr());
		p = std::copy(this->gptr(), this->gptr() + k, p);
		this->gbump(k);
	}
	return result;
}

template < typename Char, typename Traits>
typename BasicDelimiterInputBuffer<Char, Traits>::int_type
BasicDelimiterInputBuffer<Char, Traits>::underflow()
{
	if(this->gptr() < this->egptr()) return traits_type::to_int_type(*this->gptr());
	else if( ! m_is) return eof();
	else {
		int_type dl = traits_type::to_int_type(m_delimiter);
		int_type cr = traits_type::to_int_type(this->carriage_return());
		int_type nl = traits_type::to_int_type(this->newline());
		int_type qt = traits_type::to_int_type(this->quote());
		int_type ch;

		enum { QuoteNone, QuoteBegin, QuoteEnd };
		unsigned quote = 0;
		bool previouss_separator = m_separator;
		m_separator = 0;

		clear();

		// Leading spaces
		while(true) {
			ch = peek();
			if(is_eof(ch)) return eof();
			if(ch != dl && ch != nl && is_space(ch)) {
				get();
			}
			else break;
		}

		// Field
		while(true) {
			ch = get();
			if(is_eof(ch)) goto EndField;
			// Quote
			quote = QuoteNone;
			if(ch == qt) {
				quote = QuoteBegin;
				while(true) {
					ch = get();
					if(is_eof(ch)) goto EndField;
					if(ch == qt) {
						ch = get();
						if(ch != qt) goto EndQuote;
					}
					append(ch);
				}
				EndQuote:
				quote = QuoteEnd;
				if(is_eof(ch)) goto EndField;
			}

			// Carriage Return
			if(ch == cr) {
				if(peek() == nl)
					ch = get();
			}
			// New Line
			else if(ch == nl) {
				if(peek() == cr)
					get();
			}
			// Delimiter
			if(ch == dl) m_separator = field_separator();
			else if(ch == nl) m_separator = record_separator();
			if(m_separator) goto EndField;
			append(ch);
		}

		EndField:
		if(quote == QuoteBegin) {
			m_ds.setstate(std::ios_base::failbit);
			return eof();
		}
		else {
			if( ! quote) {
				// Trailing spaces
				typename string_type::iterator last = m_field.end();
				while(m_field.begin() != last) {
					if( ! is_space(*(--last))) {
						++last;
						break;
					}
				}
				m_field.erase(last, m_field.end());
			}
			m_empty = m_field.empty();
			if(m_empty && ! previouss_separator && ! quote && m_is->eof()) return eof();
			else {
				if(m_empty) m_field = m_empty_value;
				Detail::log_delimiter_stream(*this, __LINE__);
				append(m_separator);
				update_get_area();
				return traits_type::to_int_type(*this->gptr());
			}
		}
	}
}

template < typename Char, typename Traits>
typename BasicDelimiterInputBuffer<Char, Traits>::int_type
BasicDelimiterInputBuffer<Char, Traits>::pbackfail(int_type ch)
{
	// 27.6.3.4.4 Putback
	// This function is called only when:
	// - gptr() is null,
	// - gptr() == eback()
	// - or traits::eq(traits::to_char_type(c),gptr()[-1]) returns false.
	// FIXME: Do we really need this?
	if(is_eof(ch)) {
		if(this->eback() != this->gptr()) {
			this->gbump(-1);
			return traits_type::to_int_type(*this->gptr());
		}
	}
	return eof();
}


// BasicDelimiterInputStream
//=============================================================================

/// A input stream where values are separated by particular characters.
/// - Delimiters (not enclosed in quotes) are translated to the field separator.
/// - New line characters (not enclosed in quotes) are translated to the record
///   separator.
/// The only valid white space characters are the field and recoord separator.
template < typename Char, typename Traits = std::char_traits<Char> >
class BasicDelimiterInputStream	: public std::basic_istream<Char, Traits>
{
    protected:
   	typedef std::basic_istream<Char, Traits> Base;

    public:
	typedef typename Base::char_type char_type;
	typedef typename Base::int_type int_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::off_type off_type;
	typedef typename Base::traits_type traits_type;

	typedef typename BasicDelimiterInputBuffer<Char, Traits>::string_type string_type;
	typedef BasicDelimiterInputStream& (*manipulator)(BasicDelimiterInputStream&);

	/// Creates a BasicDelimiterInputStream and connects it to the given input
	/// stream.
	explicit BasicDelimiterInputStream(
		std::basic_istream<Char, Traits>& is,
		char_type delimiter = 0,
		char_type quote = 0,
		char_type carriage_return = 0,
		char_type newline = 0)
	:   Base(&m_buf),
	    m_buf(
			*this,
			is,
			delimiter,
			quote,
			carriage_return,
			newline)
	{}

	BasicDelimiterInputBuffer<Char, Traits>* rdbuf() { return &m_buf; }
	std::basic_istream<Char, Traits>& istream() { return *m_buf.istream(); }
	char_type quote() const	{ return m_buf.quote(); }
	char_type delimiter() const { return m_buf.elimiter(); }
	char_type carriage_return() const { return m_buf.carriage_return(); }
	char_type newline() const { return m_buf.newline(); }
	char_type field_separator() const	{ return m_buf.field_separator(); }
	char_type record_separator() const	{ return m_buf.record_separator(); }

	/// The empty value [Initial: Two Quotes].
	const string_type& empty_value() const { return m_buf.empty_value(); }

	/// Set the empty value.
	string_type empty_value(const string_type& value) { return m_buf.empty_value(value); }

	/// Set the empty value.
	string_type empty_value(char_type value) { return m_buf.empty_value(value); }

	/// The extracted field.
	const string_type& field() { return m_buf.field(); }

	/// \True if extracted field is replaced by the empty value.
	bool empty() const { return m_buf.empty(); }

	/// The extracted separator;
	char_type separator() const { return m_buf.separator(); }

	/// Skip a separator, if it is the next available character.
	bool skip_separator() { return m_buf.skip_separator(); }

	/// Skip a field and return one if a field is skipped, zero otherwise.
	unsigned skip_field() { return m_buf.skip_field(); }

	/// Skip the next n fields and return the number of fields skipped.
	unsigned skip_fields(unsigned n) { return m_buf.skip_fields(n); }

	/// Skip the rest of a line and return the number of fields skipped.
	unsigned skip_line() { return m_buf.skip_line(); }

	/// Match the next character with the prefix character and skip that
	/// character and the rest of a line. The function returns the number of
	/// fields skipped.
	unsigned skip_line(char_type prefix) { return m_buf.skip_line(prefix); }

	private:
	BasicDelimiterInputBuffer<Char, Traits> m_buf;
};


// Operator >>
// =============================================================================

template <typename Char, typename Traits, typename T>
inline BasicDelimiterInputStream<Char, Traits>&
operator >> (BasicDelimiterInputStream<Char, Traits>& stream, T& value) {
	static_cast<std::basic_istream<Char, Traits>&>(stream) >> value;
	return stream;
}


template <typename Char, typename Traits>
inline BasicDelimiterInputStream<Char, Traits>&
operator >> (BasicDelimiterInputStream<Char, Traits>& stream,
	typename BasicDelimiterInputStream<Char, Traits>::manipulator manip)
{
	return manip(stream);
}


/// A manipulator to skip the next field in the input stream.
/// \SEE BasicDelimiterInputStream::skip_field()
template <typename Char, typename Traits>
inline BasicDelimiterInputStream<Char, Traits>&
skip(BasicDelimiterInputStream<Char, Traits>& is)
{
	is.skip_field();
	return is;
}


// DelimiterInputStream
// ============================================================================

class DelimiterInputStream : public BasicDelimiterInputStream<char>
{
	public:
	DelimiterInputStream(std::istream& is, char_type delimiter = ',')
	:   BasicDelimiterInputStream<char>(is, delimiter)
	{}
};


// BasicDelimiterOutputBuffer
// ============================================================================


/// A stream buffer for streams holding values separated by a particular
/// characters
template <typename Char, typename Traits = std::char_traits<Char> >
class BasicDelimiterOutputBuffer : public std::basic_streambuf<Char, Traits>
{
	template <typename T>
	friend void Detail::log_delimiter_stream(
		const BasicDelimiterOutputBuffer<Char, T>&, int);

	// Types
	// =====

    private:
	typedef std::basic_streambuf<Char, Traits> Base;

    public:
	typedef typename Base::char_type char_type;
	typedef typename Base::int_type int_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::off_type off_type;
	typedef typename Base::traits_type traits_type;

	typedef typename std::basic_string<char_type> string_type;
	typedef std::pair<char_type, char_type> newline_sequence_type;

	// Construction
	// ============

    public:
	/// Connects to the given output stream.
	BasicDelimiterOutputBuffer(
		std::basic_ios<Char, Traits>& delimiter_stream,
		std::basic_ostream<Char, Traits>& os,
		char_type delimiter = 0,
		char_type quote = 0,
		char_type carriage_return = 0,
		char_type newline = 0);

	virtual ~BasicDelimiterOutputBuffer();

	// Element Access
	// ==============

    public:
	std::basic_ostream<Char, Traits>& ostream() const { return *m_os; }
	char_type delimiter() const	{ return m_delimiter; }
	char_type quote() const { return m_quote; }
	char_type carriage_return() const { return m_carriage_return; }
	char_type newline() const { return m_newline; }
	char_type field_separator() const	{ return m_dtype->field_separator(); }
	char_type record_separator() const	{ return m_dtype->record_separator(); }

	/// The newline_sequence.
	const newline_sequence_type newline_sequence() const { return m_newline_sequence; }

	/// The current field.
	const string_type& field() const { return m_field; }


    /// Set the newline_sequence.
	newline_sequence_type newline_sequence(char_type first, char_type second = 0) {
		newline_sequence_type result = m_newline_sequence;
		m_newline_sequence = newline_sequence_type(first, second);
		return result;
	}

    // Stream Buffer Interface
	// =======================

    protected:
    virtual std::streamsize xsputn(const char_type* s, std::streamsize n);
	virtual int_type overflow(int_type);
	virtual int sync();

    // Utilities
	// =========

    protected:
	int_type eof() { return traits_type::eof(); }
	int_type put(int_type ch) { return m_os->rdbuf()->sputc(ch); }
	int_type put(const char_type* p, std::streamsize n) { return m_os->rdbuf()->sputn(p, n); }
	int_type put(const string_type& s) { return m_os->rdbuf()->sputn(s.data(), s.size()); }

	void append(char_type c) { m_field += c; }
	void append(int_type ch) { append(traits_type::to_char_type(ch)); }
	void append(const string_type& s) { m_field += s; }

	bool is_eof(int_type ch) { return ch == eof(); }
	bool is_space(char_type c) { return m_dtype->forward().is(std::ctype_base::space, c); }
	bool is_space(int_type ch) { return is_space(traits_type::to_char_type(ch)); }

    private:
	std::basic_ostream<Char, Traits>* m_os;
	typename Detail::DelimiterCharacterClassification<Char>* m_dtype;
	char_type m_delimiter;
	char_type m_quote;
	char_type m_carriage_return;
	char_type m_newline;
	bool m_qt;
	bool m_fs;
	bool m_rs;
    string_type m_field;
    newline_sequence_type m_newline_sequence;
};

template < typename Char, typename Traits>
BasicDelimiterOutputBuffer<Char, Traits>::BasicDelimiterOutputBuffer(
	std::basic_ios<Char, Traits>& delimiter_stream,
	std::basic_ostream<Char, Traits>& os,
	char_type delimiter,
	char_type quote,
	char_type carriage_return,
	char_type newline)
:   m_os(&os),
    m_dtype(new Detail::DelimiterCharacterClassification<Char>(
    	std::use_facet<std::ctype<Char> >(os.getloc()))),
	m_delimiter(delimiter ? delimiter : m_dtype->widen(',')),
	m_quote(quote ? quote : m_dtype->widen('"')),
	m_carriage_return(carriage_return ? carriage_return : m_dtype->widen('\r')),
	m_newline(newline ? newline : m_dtype->widen('\n')),
    m_qt(false),
    m_fs(false),
    m_rs(false),
    m_newline_sequence(m_carriage_return, m_newline)
{}

// Logging
// =======

namespace Detail {
	template<typename Char, typename Traits>
	inline void log_delimiter_stream(
		const BasicDelimiterOutputBuffer<Char, Traits>&,
		int line)
	{}

	#if POCO_LOG_DELIMITER_OUTPUT_BUFFER_ENABLE
	template<typename Traits>
	inline void log_delimiter_stream(
		const BasicDelimiterOutputBuffer<char, Traits>& sb,
		int line)
	{
		std::clog << line
			<< " Output: "
			<< ((sb.m_qt) ? 'q' : '-')
			<< ((sb.m_fs) ? 'f' : '-')
			<< ((sb.m_rs) ? 'r' : '-')
			<< ' '
			<< sb.field()
			<< std::endl;
	}
	#endif
}

// Put Area
// ========

template < typename Char, typename Traits>
BasicDelimiterOutputBuffer<Char, Traits>::~BasicDelimiterOutputBuffer() {
	if( ! m_field.empty()) {
		sync();
	}
}

template < typename Char, typename Traits>
std::streamsize
BasicDelimiterOutputBuffer<Char, Traits>::xsputn(const char_type* s, std::streamsize n) {
	char_type cr = this->carriage_return();
	char_type nl = this->newline();
	char_type qt = this->quote();
	char_type fs = this->field_separator();
	char_type rs = this->record_separator();

	for(std::streamsize i = 0; i < n; ++i) {
		if(s[i] == fs || s[i] == rs) {
			if(s[i] == fs) m_fs = true;
			else m_rs = true;
			if(is_eof(sync())) {
				return eof();
			}
		}
		else {
			if(s[i] == nl) m_qt = true;
			else if(s[i] == qt) {
				append(qt);
				m_qt = true;
			}
			append(s[i]);
		}
	}
	return n;
}

template < typename Char, typename Traits>
typename BasicDelimiterOutputBuffer<Char, Traits>::int_type
BasicDelimiterOutputBuffer<Char, Traits>::overflow(int_type ch)
{
	if(is_eof(ch)) return eof();
	else {
		char_type c = traits_type::to_char_type(ch);
		return xsputn(&c, 1);
	}
}

template < typename Char, typename Traits>
int BasicDelimiterOutputBuffer<Char, Traits>::sync() {
	char_type qt = this->quote();

	if(m_field.empty())	Detail::log_delimiter_stream(*this, __LINE__);
	else {
		// Quote
		if(is_space(m_field[0]) || is_space(m_field[m_field.size()-1]))
			m_qt = true;
		if(m_qt && is_eof(put(qt))) return eof();

		// Buffer
		std::streamoff offset = 0;
		Detail::log_delimiter_stream(*this, __LINE__);
		while(offset < std::streamoff(m_field.size())) {
			std::streamoff k = put(m_field.data() + offset, m_field.size() - offset);
			if(0 <= k) offset += k;
			else {
				m_field.erase(m_field.begin(), m_field.begin() + offset);
				return eof();
			}
		}
		m_field.clear();
	}
	// Quote
	if(m_qt && is_eof(put(qt))) return eof();
	m_qt = false;

	if(m_fs) {
		if(is_eof(put(m_delimiter))) return eof();
		m_fs = false;
	}
	else if(m_rs)  {
		if(is_eof(put(m_newline_sequence.first))) return eof();
		if(m_newline_sequence.second && is_eof(put(m_newline_sequence.second))) return eof();
		m_rs = false;
	}
	return 0;
}


// BasicDelimiterOutputStream
//=============================================================================

/// An output stream for writing values separated by the delimiter character.
template < typename Char, typename Traits = std::char_traits<Char> >
class BasicDelimiterOutputStream : public std::basic_ostream<Char, Traits>
{
    protected:
   	typedef std::basic_ostream<Char, Traits> Base;

    public:
	typedef typename Base::char_type char_type;
	typedef typename Base::int_type int_type;
	typedef typename Base::pos_type pos_type;
	typedef typename Base::off_type off_type;
	typedef typename Base::traits_type traits_type;
	typedef typename BasicDelimiterInputBuffer<Char, Traits>::string_type string_type;
	typedef BasicDelimiterOutputStream& (*manipulator)(BasicDelimiterOutputStream&);

	// Construction
	// ============

    public:
	/// Creates a BasicDelimiterOutputStream and connects it to the given
	/// output stream.
	BasicDelimiterOutputStream(
		std::basic_ostream<Char, Traits>& os,
		char_type delimiter = 0,
		char_type quote = 0,
		char_type carriage_return = 0,
		char_type newline = 0)
	:   Base(&m_buf),
	    m_buf(
			*this,
			os,
			delimiter,
			quote,
			carriage_return,
			newline)
	{}

	// Element Access
	// ==============

    public:
	std::basic_ostream<Char, Traits>& ostream() const { return *m_buf.ostream(); }
	char_type quote() const	{ return m_buf.quote(); }
	char_type delimiter() const { return m_buf.elimiter(); }
	char_type carriage_return() const { return m_buf.carriage_return(); }
	char_type newline() const { return m_buf.newline(); }
	char_type field_separator() const { return m_buf.field_separator(); }
	char_type record_separator() const { return m_buf.record_separator(); }

	/// The newline_sequence.
	const string_type& newline_sequence() const { return m_buf.newline_sequence(); }

	/// Set the newline_sequence.
	string_type newline_sequence(const string_type& value) { return m_buf.newline_sequence(value); }

	/// Set the newline_sequence.
	string_type newline_sequence(char_type value) { return m_buf.newline_sequence(value); }

	/// The current field.
	const string_type& field() { return m_buf.field(); }

    private:
    BasicDelimiterOutputBuffer<Char, Traits> m_buf;
};



// Operator <<
// =============================================================================

template <typename Char, typename Traits, typename T>
inline BasicDelimiterOutputStream<Char, Traits>&
operator << (BasicDelimiterOutputStream<Char, Traits>& stream, const T& value) {
	static_cast<std::basic_ostream<Char, Traits>&>(stream) << value;
	return stream;
}


template <typename Char, typename Traits>
inline BasicDelimiterOutputStream<Char, Traits>&
operator << (BasicDelimiterOutputStream<Char, Traits>& stream,
	typename BasicDelimiterOutputStream<Char, Traits>::manipulator manip)
{
	return manip(stream);
}

/// A manipulator for putting a field separator in the output stream.
/// \SEE BasicDelimiterOutputStream::field_separator()
template <typename Char, typename Traits>
inline BasicDelimiterOutputStream<Char, Traits>&
fs(BasicDelimiterOutputStream<Char, Traits>& os)
{
	os.put(os.field_separator());
	return os;
}

/// A manipulator for putting a record separator in the output stream.
/// \SEE BasicDelimiterOutputStream::record_separator()
template <typename Char, typename Traits>
inline BasicDelimiterOutputStream<Char, Traits>&
rs(BasicDelimiterOutputStream<Char, Traits>& os)
{
	os.put(os.record_separator());
	return os;
}


// DelimiterOutputStream
// ============================================================================

class DelimiterOutputStream : public BasicDelimiterOutputStream<char>
{
	public:
	DelimiterOutputStream(std::ostream& os, char_type delimiter = ',')
	:   BasicDelimiterOutputStream<char>(os, delimiter)
	{}
};

} // namespace Poco
#endif

