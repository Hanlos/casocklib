/*
 * casocklib - An asynchronous communication library for C++
 * ---------------------------------------------------------
 * Copyright (C) 2010 Leandro Costa
 *
 * This file is part of casocklib.
 *
 * casocklib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as
 * published by the Free Software Foundation, either version 3 of
 * the License, or (at your option) any later version.
 *
 * casocklib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with casocklib. If not, see <http://www.gnu.org/licenses/>.
 */

/*!
 * \file tests/casock/rpc/protobuf/client/test_RPCCallTimeoutHandler.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include <gtest/gtest.h>

#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/rpc/protobuf/client/RPCCall.h"
#include "casock/rpc/protobuf/client/RPCCallHash.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"
#include "casock/rpc/protobuf/client/RPCCallController.h"
#include "casock/rpc/protobuf/client/RPCCallTimeoutHandler.h"

#include "tests/casock/rpc/protobuf/client/RPCCallHandlerFactoryStub.h"
#include "tests/casock/rpc/protobuf/client/RPCClientProxyTest.h"

TEST(casock_rpc_protobuf_client_RPCCallTimeoutHandler, Basic) {
  casock::rpc::protobuf::client::RPCCallHash hash;
  casock::rpc::protobuf::client::RPCCallQueue queue;
  casock::rpc::protobuf::client::RPCCallTimeoutHandler handler (hash, queue);

  handler.run ();

  uint32 id = 0;
  struct timeval timeout;
  timeout.tv_sec = 0;
  timeout.tv_usec = 0;

  tests::casock::rpc::protobuf::client::RPCCallHandlerFactoryStub factory;
  tests::casock::rpc::protobuf::client::RPCClientProxyTest proxy (factory);
  casock::rpc::protobuf::client::RPCCallController* pController = proxy.buildRPCCallController ();
  casock::rpc::protobuf::client::RPCCall *pCall = new casock::rpc::protobuf::client::RPCCall (NULL, pController, NULL);
  hash.push (id, pCall);

  handler.registerTimeout (id, pController->timeout ());
  usleep (1000000 * pController->timeoutInUSeconds ());

  //EXPECT_TRUE (pController->Failed ());
}

int main (int argc, char **argv) {
  ::testing::InitGoogleTest (&argc, argv);
  return RUN_ALL_TESTS();
}
