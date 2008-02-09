#
# Makefile
#
# $Id: //poco/Main/Script/Lua/Makefile#2 $
#
# Makefile for Poco Lua
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I $(POCO_BASE)/Script/Lua/include/Poco/Script/Lua/lua

ifeq ($(OSNAME),Darwin)
COMMONFLAGS += -DLUA_USE_MACOSX
else
COMMONFLAGS += -DLUA_USE_LINUX
endif

objects = Environment \
	lapi lauxlib lbaselib lcode ldblib ldebug ldo ldump lfunc lgc linit \
	liolib llex lmathlib lmem loadlib lobject lopcodes loslib lparser \
	lstate lstring lstrlib ltable ltablib ltm lundump lvm lzio print

target         = PocoLua
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoScript

include $(POCO_BASE)/build/rules/lib
