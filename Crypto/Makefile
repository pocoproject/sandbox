#
# Makefile
#
# $Id$
#
# Makefile for Poco Crypto
#

include $(POCO_BASE)/build/rules/global

SYSLIBS += -lssl -lcrypto

objects = Cipher CipherFactory CipherImpl CryptoStream CryptoTransform \
	RSAKey RSAKeyImpl RSADigestEngine

target         = PocoCrypto
target_version = $(LIBVERSION)
target_libs    = PocoNetSSL PocoNet PocoFoundation

include $(POCO_BASE)/build/rules/lib
