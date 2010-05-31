CXXTSRCS_RPC_CLIENT_PROXY = test_RPCClientProxy_cxx.cc
CXXTSRCS = $(CXXTSRCS_RPC_CLIENT_PROXY)

SRCS_RPC_CLIENT_PROXY = $(CXXTSRCS_RPC_CLIENT_PROXY) \
												RPCCallHandlerMock.cc RPCCallHandlerFactoryMock.cc

LIB_UTIL      = $(top_srcdir)/src/casock/util/libcasutil.la
LIB_RPCCLIENT = $(top_srcdir)/src/casock/rpc/protobuf/client/libcasrpcpbclient.la
LIB_PROTOBUF  = @LIB_PROTOBUF_PATH@/libprotobuf.a
LIB_RPCTEST1  = $(top_srcdir)/src/tests/rpc/protobuf/api/libcasrpcpbtapi.la

AM_CPPFLAGS = -DUSE_UT_CXXTEST

check_PROGRAMS = test_RPCClientProxy_cxx

test_RPCClientProxy_cxx_SOURCES = $(SRCS_RPC_CLIENT_PROXY)
test_RPCClientProxy_cxx_LDADD		= $(LIB_RPCCLIENT) $(LIB_UTIL) $(LIB_RPCTEST1) $(LIB_PROTOBUF)
test_RPCClientProxy_cxx_LDFLAGS = -static

TESTS	= $(check_PROGRAMS)

%_cxx.cc: %_cxx.h
	$(CXXTESTGEN) $(CXXTFLAGS) $< -o $@

CLEANFILES = $(CXXTSRCS)
