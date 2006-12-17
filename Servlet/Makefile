#
# Makefile
#

#
# Makefile for Molto
#

ifndef POCO_BASE
$(warning WARNING: POCO_BASE is not defined. Assuming one level below current directory.)
export POCO_BASE=$(shell pwd)/..
endif

.DEFAULT all:
	$(MAKE) -C Servlet $(MAKECMDGOALS)
	$(MAKE) -C Container $(MAKECMDGOALS)
	$(MAKE) -C Molto $(MAKECMDGOALS)
	$(MAKE) -C PocoServer $(MAKECMDGOALS)
	$(MAKE) -C testsuite $(MAKECMDGOALS)

	
