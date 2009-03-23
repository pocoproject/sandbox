#
# Makefile
#
# $Id: //poco/svn/Sockets/testsuite/Makefile#1 $
#
# Makefile for Poco Sockets testsuite
#

include $(POCO_BASE)/build/rules/global

objects = \
	DNSTest MulticastSocketTest SocketStreamTest \
	DatagramSocketTest DatagramLocalSocketTest SocketTest \
	LocalSocketTest Driver SocketsTestSuite EchoServer \
	AddressTest AddressTestSuite \
	SocketTestSuite UDPEchoServer UDPLocalEchoServer \
	NetworkInterfaceTest \
	MulticastEchoServer SocketAddressTest \
	DialogSocketTest DialogServer RawSocketTest

target         = testrunner
target_version = 1
target_libs    = PocoSockets PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
