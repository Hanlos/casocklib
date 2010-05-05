RPC_PROTO = rpc_test1.proto
RPC_PB_H  = rpc_test1.pb.h
RPC_PB_C  = rpc_test1.pb.cc

noinst_LTLIBRARIES = libcasrpcpbt1api.la

libcasrpcpbt1api_la_SOURCES = $(RPC_PB_C)
BUILT_SOURCES               = $(RPC_PB_C)

$(RPC_PB_C): $(RPC_PROTO)
	$(PROTOC) $^ --cpp_out=.

CLEANFILES = $(RPC_PB_H) $(RPC_PB_C)
