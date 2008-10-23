#
# Makefile
#
# $Id: //poco/Main/template/TestSuite.make#3 $
#
# Makefile for Poco IO testsuite
#

include $(POCO_BASE)/build/rules/global

objects = DeviceIOTestSuite Driver \
	AsyncChannelTest AsyncDeviceIOTestSuite \
	ProtocolTest ProtocolTestSuite TestProtocol TestChannel

target         = testrunner
target_version = 1
target_libs    = PocoDeviceIO PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
