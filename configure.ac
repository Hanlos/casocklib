#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ(2.60)
AC_INIT(casocklib, 0.01, leandro.costa@gmail.com)
AC_CONFIG_SRCDIR([config.h.in])
AC_CONFIG_HEADER([config.h])
AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX
AC_PROG_CC
AC_PROG_CPP
AC_PROG_INSTALL
AC_PROG_LN_S
AC_PROG_MAKE_SET
AC_PROG_RANLIB
AC_PROG_LIBTOOL

# Checks for libraries.
# FIXME: Replace `main' with a function in `-lprotobuf':
AC_CHECK_LIB([protobuf], [main])
# FIXME: Replace `main' with a function in `-lpthread':
AC_CHECK_LIB([pthread], [main])

# Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS([arpa/inet.h fcntl.h netdb.h netinet/in.h stdlib.h string.h strings.h sys/ioctl.h sys/socket.h sys/time.h unistd.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_HEADER_STDBOOL
AC_C_CONST
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_SSIZE_T

# Checks for library functions.
AC_CHECK_FUNCS([alarm bzero gethostbyname gettimeofday localtime_r memset select socket strerror])


# Default directories

DIR_PROTOBUF=/usr/local/protobuf-2.3.0
#DIR_PROTOBUF=/usr/local/protobuf-2.2.0a
DIR_PROTOBUF_LIB_DEFAULT=$DIR_PROTOBUF/lib
DIR_PROTOBUF_INC_DEFAULT=$DIR_PROTOBUF/include


# Tools

#   protoc: Google Protocol Buffers (http://code.google.com/p/protobuf)
AC_PATH_PROG([PROTOC], [protoc], [protoc], [$DIR_PROTOBUF/bin])


# Includes and compiler directives

INCLUDES='-I$(top_srcdir)/src'
CPPFLAGS='-g -Wall -O -Wno-write-strings -D_REENTRANT -D_GLIBCXX_DEBUG -D_GLIBCXX_DEBUG_PEDANTIC'
CFLAGS=
CXXFLAGS=


dnl **********************************************************************
dnl Check for presence of Google Protocol Buffers libraries and headers
dnl **********************************************************************

AC_ARG_WITH(protobuf-includes,
            AC_HELP_STRING([--with-protobuf-includes=DIR],
                           [Specify location of Google Protocol Buffers headers]),
                           [INCLUDES="$INCLUDES -I$withval"],
                            [INCLUDES="$INCLUDES -I${DIR_PROTOBUF_INC_DEFAULT}"])

AC_ARG_WITH(protobuf-libs,
            AC_HELP_STRING([--with-protobuf-libs=DIR],
                           [Specify location of Google Protocol Buffers libs]),
                           [LIB_PROTOBUF_PATH="$withval"],
                           [LIB_PROTOBUF_PATH="${DIR_PROTOBUF_LIB_DEFAULT}"])

AC_SUBST(LIB_PROTOBUF_PATH)
AC_SUBST(INCLUDES)

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/casock/Makefile
                 src/casock/proactor/Makefile
                 src/casock/proactor/asio/Makefile
                 src/casock/proactor/asio/base/Makefile
                 src/casock/proactor/asio/client/Makefile
                 src/casock/proactor/asio/server/Makefile
                 src/casock/rpc/Makefile
                 src/casock/rpc/asio/Makefile
                 src/casock/rpc/asio/protobuf/Makefile
                 src/casock/rpc/asio/protobuf/base/Makefile
                 src/casock/rpc/asio/protobuf/client/Makefile
                 src/casock/rpc/asio/protobuf/server/Makefile
                 src/casock/rpc/protobuf/Makefile
                 src/casock/rpc/protobuf/api/Makefile
                 src/casock/rpc/protobuf/client/Makefile
                 src/casock/rpc/protobuf/server/Makefile
                 src/casock/rpc/sigio/Makefile
                 src/casock/rpc/sigio/protobuf/Makefile
                 src/casock/rpc/sigio/protobuf/base/Makefile
                 src/casock/rpc/sigio/protobuf/client/Makefile
                 src/casock/rpc/sigio/protobuf/server/Makefile
                 src/casock/sigio/Makefile
                 src/casock/sigio/base/Makefile
                 src/casock/sigio/client/Makefile
                 src/casock/sigio/server/Makefile
                 src/casock/util/Makefile
                 src/examples/Makefile
                 src/examples/ftp/Makefile
                 src/examples/proactor/Makefile
                 src/examples/proactor/asio/Makefile
                 src/examples/proactor/asio/ftp/Makefile
                 src/examples/rpc/Makefile
                 src/examples/rpc/asio/Makefile
                 src/examples/rpc/protobuf/Makefile
                 src/examples/rpc/protobuf/api/Makefile
                 src/examples/rpc/sigio/Makefile])
AC_OUTPUT
