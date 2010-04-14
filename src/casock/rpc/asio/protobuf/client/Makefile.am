SRCS = RPCClientProxy.cc RPCSocketClient.cc # RPCChannel.cc RPCReaderHandler.cc RPCClientCommunicator.cc

LIBS = $(top_srcdir)/src/casock/proactor/asio/client/libcasproasioclient.la \
			 $(top_srcdir)/src/casock/rpc/asio/protobuf/base/libcasrpcaiopbbase.la \
       $(top_srcdir)/src/casock/rpc/protobuf/api/libcasrpcpbapi.la

noinst_LTLIBRARIES = libcasrpcaiopbclient.la

libcasrpcaiopbclient_la_SOURCES  = $(SRCS)
libcasrpcaiopbclient_la_LIBADD   = $(LIBS)
