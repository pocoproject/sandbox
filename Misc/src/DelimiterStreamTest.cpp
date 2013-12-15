// vi:set ts=4 sw=4 nowrap:

#include <iostream>
#include <sstream>

#define POCO_LOG_DELIMITER_INPUT_BUFFER_ENABLE 1
#define POCO_LOG_DELIMITER_OUTPUT_BUFFER_ENABLE 1
#include "delimiter_stream.h"


// Test
// =============================================================================

namespace {

#define STRINGS \
	"#Comment\n" \
	"#Comment\n" \
	"#Comment\n" \
	"'''#No Comment Inside Quotes''',#Comment\n" \
	"Skip,\n" \
	"Extra Spaces After Flush\n" \
	"Strings:\n" \
	",, "" \n" \
	"Empty Quotes''," \
	"Double Quotes''''\n" \
	"'   Leading and Trailing Space   '," \
	"   'Trimmed Leading and Trailing Space'   \n" \
	"'''A , Comma'''\n" \
	"'''First ...''\n''... Second Line'''\n" \
	"Numbers:\n"

#define NUMBERS \
	"   1.23   ,,'', '1 2'," \
	"The space in the previous quoted field will lead to a failure."


#define EMPTY_STRING "[Empty]"

#define RESULT \
	"'''#No Comment Inside Quotes'''\r\n" \
	"Extra Spaces After Flush'   '\r\n" \
	"Strings:\r\n" \
	",,\r\n" \
	"Empty Quotes,'Double Quotes'''\r\n" \
    "'   Leading and Trailing Space   ',Trimmed Leading and Trailing Space\r\n" \
	"'''A , Comma'''\r\n" \
	"'''First ...''\n" \
	"''... Second Line'''\r\n" \
	"Numbers:\r\n" \
	"1.23,0,0,1,Failure,"

#define W(S) W2(S)
#define W2(S) L##S

template <typename Char>
std::basic_string<Char> test(
	const Char* strings,
	const Char* numbers,
	const Char* empty_string)
{
	std::basic_stringstream<Char> input(strings);
	std::basic_stringstream<Char> output;
	const std::ctype<Char>& ctype = std::use_facet<std::ctype<Char> >(input.getloc());
	const Char qt = ctype.widen('\'');
	const Char ws = ctype.widen(' ');
	const Char comment_character = ctype.widen('#');
	const Char empty_number = ctype.widen('0');

	Poco::BasicDelimiterInputStream<Char> in(input, 0, qt);
	Poco::BasicDelimiterOutputStream<Char> out(output, 0, qt);

	using Poco::fs;
	using Poco::rs;

	std::basic_string<Char> s;

	while(in.skip_line(comment_character));
	if(in >> s) {
		out << s;
		in.skip_line(comment_character);
		if(in.separator()) out << in.separator();
	}

	if(in.skip_fields(1) == 1) {
		in.skip_line();
	}

	if(in >> s) {
		out << s;
		out.flush();
		out << ws << ws << ws;
		if(in.separator()) out << in.separator();
	}

	in.empty_value(empty_string);
	while(true) {
		if( ! (in >> s)) break;
		else {
			if( ! in.empty()) out << s;
			if(in.separator()) out << in.separator();
		}
	}

	in.clear();
	in.empty_value(empty_number);
	input.str(numbers);
	double f;
	while(true) {
		if( ! (in >> f)) {
			out << "Failure";
			if(in.separator()) out << in.separator();
			break;
		}
		else {
			out << f;
			if(in.separator()) out << in.separator();
		}
	}
	out.flush();
	return output.str();
}


} // namespace


int main() {
	std::string s = test<char>(
		STRINGS,
		NUMBERS,
		EMPTY_STRING);
	bool s_success = (s == RESULT);
	if( ! s_success) {
		std::clog << "String Test Failure" << '\n';
		const char* r = RESULT;
		const char* r_end = r + std::min(sizeof(RESULT), s.size());
		std::pair<const char*, std::string::const_iterator> m = std::mismatch(r, r_end, s.begin());
		if(m.first != r_end) {
			std::string::size_type pos = m.first - r;
			std::clog
			    << "Mismatch At:\n"
				<< "Expected:\n[0x" << std::hex << (unsigned)m.first[0] << "] "
				<< m.first << '\n'  << '\n'
				<< "Available:\n[0x" << std::hex << (unsigned)s[pos]  << "] "
				<< s.c_str() + pos << '\n' << '\n';
		}
	}

	std::wstring w = test<wchar_t>(
		W(STRINGS),
		W(NUMBERS),
		W(EMPTY_STRING));
	bool w_success = (w == W(RESULT));
	if( ! w_success)
		std::clog << "Wide String Test Failure" << '\n';

	// std::clog << "Output\n" << s << '\n';
	return s_success && w_success;
}

