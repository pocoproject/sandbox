#
# Makefile
#
#
# Makefile for Servlet Container
#

include $(POCO_BASE)/build/rules/global

objects = ConfigImpl WebContainer HttpSessionImpl SessionManagerImpl \
	WebApplicationTask WebServerTask Contained FilterChainImpl \
	ServletContextImpl WebApplication WebServer

target         = Container
target_version = 1
target_libs    = PocoFoundation PocoXML PocoUtil Servlet ServletEx

include $(POCO_BASE)/build/rules/lib
