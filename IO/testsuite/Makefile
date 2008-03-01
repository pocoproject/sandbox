#
# Makefile
#
# $Id: //poco/Main/template/TestSuite.make#3 $
#
# Makefile for Poco IO testsuite
#

include $(POCO_BASE)/build/rules/global

objects = IOTestSuite Driver \
	SerialTestSuite SerialTestHW  SerialTestHWSuite  SerialTestSW  SerialTestSWSuite \
	ProtocolTest ProtocolTestSuite TestProtocol

target         = testrunner
target_version = 1
target_libs    = PocoIO PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
