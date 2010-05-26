SRCS = RPCClientProxy.cc RPCSocketClient.cc RPCSocketClientImpl.cc RPCSocketClientFactoryImpl.cc RPCClientCommunicator.cc RPCChannel.cc

LIBS = $(top_srcdir)/src/casock/proactor/asio/client/libcasproasioclient.la \
			 $(top_srcdir)/src/casock/rpc/asio/protobuf/base/libcasrpcaiopbbase.la \
       $(top_srcdir)/src/casock/rpc/protobuf/client/libcasrpcpbclient.la

noinst_LTLIBRARIES = libcasrpcaiopbclient.la

libcasrpcaiopbclient_la_SOURCES  = $(SRCS)
libcasrpcaiopbclient_la_LIBADD   = $(LIBS)
