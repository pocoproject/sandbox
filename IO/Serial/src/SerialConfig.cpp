//
// SerialConfig.cpp
//
// $Id: //poco/Main/IO/Serial/src/SerialConfig.cpp#1 $
//
// Library: Serial
// Package: Serial
// Module:  SerialConfig
//
// Copyright (c) 2008, Applied Informatics Software Engineering GmbH.
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


#include "Poco/IO/Serial/SerialConfig.h"


#if defined(POCO_OS_FAMILY_WINDOWS)
	#include "SerialConfig_WIN32.cpp"
#elif defined(POCO_OS_FAMILY_UNIX)
	#include "SerialConfig_POSIX.cpp"
#endif


namespace Poco {
namespace IO {
namespace Serial {


const unsigned char SerialConfig::DEFAULT_XON=0x02;
const unsigned char SerialConfig::DEFAULT_XOFF=0x03;
const unsigned char SerialConfig::DEFAULT_EOF=0x04;


SerialConfig::SerialConfig(const std::string& name): SerialConfigImpl(SerialConfigImpl::BPS_2400_IMPL, 
	SerialConfigImpl::DATA_BITS_EIGHT_IMPL, 
	'N', 
	SerialConfigImpl::START_ONE_IMPL, 
	SerialConfigImpl::STOP_ONE_IMPL,
	SerialConfigImpl::FLOW_CTRL_HARDWARE_IMPL,
	0, 
	0,
	true,
	0,
	1,
	getTimeout())
{
}


SerialConfig::SerialConfig(const std::string& name,
	BPSRate bpsRate,
	DataBits dataBits,
	char parity,
	StartBits startBits,
	StopBits stopBits,
	FlowControl flowControl,
	unsigned char xOnChar,
	unsigned char xOffChar,
	bool useEOF,
	unsigned char eof,
	int bufferSize,
	int timeout):
	ChannelConfig(name, ChannelConfig::CONNECTION_CHANNEL, timeout),
	SerialConfigImpl(static_cast<SerialConfigImpl::BPSRateImpl>(bpsRate),
		static_cast<SerialConfigImpl::DataBitsImpl>(dataBits),
		parity,
		static_cast<SerialConfigImpl::StartBitsImpl>(startBits),
		static_cast<SerialConfigImpl::StopBitsImpl>(stopBits),
		static_cast<SerialConfigImpl::FlowControlImpl>(flowControl),
		xOnChar,
		xOffChar,
		useEOF,
		eof,
		bufferSize,
		getTimeout())
{
}


void SerialConfig::setBPSRate(SerialConfig::BPSRate bpsRate)
{
	if (SerialConfigImpl::NOT_SUPPORTED == bpsRate)
		throw InvalidArgumentException("Baud rate not supported.");

	setBPSRateImpl((SerialConfigImpl::BPSRateImpl) bpsRate);
}


void SerialConfig::setXonChar(unsigned char c)
{
	if (SerialConfigImpl::FLOW_CTRL_HARDWARE_IMPL == getFlowControlImpl())
		throw InvalidAccessException("Can not set XON character for hardware flow control.");

	setXonCharImpl(c);
}


void SerialConfig::setXoffChar(unsigned char c)
{
	if (SerialConfigImpl::FLOW_CTRL_HARDWARE_IMPL == getFlowControlImpl())
		throw InvalidAccessException("Can not set XOFF character for hardware flow control.");

	setXoffCharImpl(c);
}


} } } // namespace Poco::IO::Serial
