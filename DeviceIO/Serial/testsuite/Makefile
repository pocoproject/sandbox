#
# Makefile
#
# $Id: //poco/Main/template/TestSuite.make#3 $
#
# Makefile for Poco Serial testsuite
#

include $(POCO_BASE)/build/rules/global

objects = SerialTestSuite Driver \
	SerialTestHW  SerialTestHWSuite  SerialTestSW  SerialTestSWSuite \
	ProtocolTest ProtocolTestSuite TestProtocol

target         = testrunner
target_version = 1
target_libs    = PocoSerial PocoDeviceIO PocoFoundation CppUnit

include $(POCO_BASE)/build/rules/exec
