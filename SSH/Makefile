#
# Makefile
#
# $Id: //poco/Main/SSH/Makefile#3 $
#
# Makefile for Poco SSH
#

include $(POCO_BASE)/build/rules/global

INCLUDE += -I./src

SYSLIBS += -lssl -lcrypto

objects = channel comp crypt DummyValidator \
	hostkey kex mac misc openssl packet scp \
	SCPReadOperation SCPStream SCPWriteOperation \
	session sftp SSHChannel SSHChannelImpl \
	SSHChannelStream SSHException SSHHostValidator \
	SSHOperation SSHReadOperation SSHSession SSHSessionImpl \
	SSHSocketImpl SSHStreamSocket SSHStreamSocketImpl \
	SSHTerminal SSHWriteOperation transport userauth \
	SSHSubSystem

target         = PocoSSH
target_version = $(LIBVERSION)
target_libs    = PocoFoundation PocoNet

include $(POCO_BASE)/build/rules/lib
