CXXTSRCS_TEST1  = test1_cxx.cc
CXXTSRCS        = $(CXXTSRCS_TEST1)
SRCS_TEST1      = $(CXXTSRCS_TEST1)# $(top_builddir)/src/casocklib/rpc/asio/protobuf/RPCClientProxy.h

AM_CPPFLAGS = -DUSE_UT_CXXTEST

check_PROGRAMS    = test1_cxx
test1_cxx_SOURCES = $(SRCS_TEST1)
TESTS             = $(check_PROGRAMS)

%_cxx.cc: %_cxx.h
	$(CXXTESTGEN) $(CXXTFLAGS) $< -o $@

CLEANFILES = $(CXXTSRCS)
