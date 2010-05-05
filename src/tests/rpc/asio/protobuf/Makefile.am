CXXTSRCS_TEST1  = test1_cxx.cc
CXXTSRCS        = $(CXXTSRCS_TEST1)
SRCS_TEST1      = $(CXXTSRCS_TEST1) # $(top_builddir)/src/casocklib/rpc/asio/protobuf/RPCClientProxy.h

LIB_UTIL      = $(top_srcdir)/src/casock/util/libcasutil.la
LIB_RPCCLIENT = $(top_srcdir)/src/casock/rpc/asio/protobuf/client/libcasrpcaiopbclient.la
LIB_RPCSERVER = $(top_srcdir)/src/casock/rpc/asio/protobuf/server/libcasrpcaiopbserver.la
LIB_PROTOBUF  = @LIB_PROTOBUF_PATH@/libprotobuf.a
LIB_RPCTEST1  = $(top_srcdir)/src/tests/rpc/protobuf/api/libcasrpcpbt1api.la

AM_CPPFLAGS = -DUSE_UT_CXXTEST

check_PROGRAMS    = test1_cxx
test1_cxx_SOURCES = $(SRCS_TEST1)
test1_cxx_LDADD   = $(LIB_RPCCLIENT) $(LIB_RPCSERVER) $(LIB_UTIL) $(LIB_RPCTEST1) $(LIB_PROTOBUF)
test1_cxx_LDFLAGS = -static
TESTS             = $(check_PROGRAMS)

%_cxx.cc: %_cxx.h
	$(CXXTESTGEN) $(CXXTFLAGS) $< -o $@

CLEANFILES = $(CXXTSRCS)
