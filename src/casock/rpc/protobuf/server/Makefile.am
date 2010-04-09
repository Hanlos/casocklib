SRCS = RPCServerProxy.cc

LIBS = $(top_srcdir)/src/casock/rpc/protobuf/api/libcasrpcpbapi.la

noinst_LTLIBRARIES = libcasrpcpbserver.la

libcasrpcpbserver_la_SOURCES  = $(SRCS)
libcasrpcpbserver_la_LIBADD   = $(LIBS)
