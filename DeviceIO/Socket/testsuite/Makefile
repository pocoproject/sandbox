#
# Makefile
#
# $Id: //poco/Main/template/TestSuite.make#3 $
#
# Makefile for Poco IO testsuite
#

include $(POCO_BASE)/build/rules/global

objects = SocketTestSuite Driver \
	SocketChannelTest UDPEchoServer

target         = testrunner
target_version = 1
target_libs    = PocoSocket PocoDeviceIO PocoFoundation PocoNet CppUnit

include $(POCO_BASE)/build/rules/exec
