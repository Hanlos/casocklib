SRCS = RPCServerProxy.cc RPCSocketSession.cc

LIBS = $(top_srcdir)/src/casock/proactor/asio/server/libcasproasioserver.la

noinst_LTLIBRARIES = libcasprorpcserver.la

libcasprorpcserver_la_SOURCES = $(SRCS)
libcasprorpcserver_la_LIBADD  = $(LIBS)
