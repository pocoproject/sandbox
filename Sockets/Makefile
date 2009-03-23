#
# Makefile
#
# $Id: //poco/svn/Sockets/Makefile#2 $
#
# Makefile for Poco Sockets
#

include $(POCO_BASE)/build/rules/global

SHAREDOPT_CXX   += -DSockets_EXPORTS

objects = DNS HostEntry NetworkInterface Address \
	SocketAddress Socket DatagramSocket \
	DialogSocket DatagramSocketImpl MulticastSocket \
	SocketStream StreamSocket SocketImpl StreamSocketImpl \
	SocketException ServerSocket ServerSocketImpl \
	RawSocket RawSocketImpl

target         = PocoSockets
target_version = $(LIBVERSION)
target_libs    = PocoFoundation

include $(POCO_BASE)/build/rules/lib
