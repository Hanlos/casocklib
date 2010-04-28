SRCS = RPCClientProxy.cc RPCCallHandler.cc

LIBS = $(top_srcdir)/src/casock/rpc/protobuf/api/libcasrpcpbapi.la

noinst_LTLIBRARIES = libcasrpcpbclient.la

libcasrpcpbclient_la_SOURCES  = $(SRCS)
libcasrpcpbclient_la_LIBADD   = $(LIBS)
