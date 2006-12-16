#
# Makefile
#
#
# Makefile for Molto Test suite
#

include $(POCO_BASE)/build/rules/global

objects = ContainerTest Driver PocoServerTest \
	ServletExTest ServletTest ServletTestSuite SessionTest 

target         = testrunner
target_version = 1
target_libs    = PocoFoundation PocoXML PocoUtil PocoNet Servlet ServletEx Container PocoServer TestServlet TestFilter CppUnit

include $(POCO_BASE)/build/rules/exec
