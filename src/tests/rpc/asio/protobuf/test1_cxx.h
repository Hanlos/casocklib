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
 * \file tests/rpc/asio/protobuf/test1_cxx.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */


#ifndef __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST1_CXX_H_
#define __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST1_CXX_H_

#include <cxxtest/TestSuite.h>
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/asio/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/asio/protobuf/server/RPCServerProxy.h"
#include "tests/rpc/asio/protobuf/Test1ServiceImpl.h"
#include "tests/rpc/asio/protobuf/Test1ResponseHandlerImpl.h"
#include "tests/rpc/protobuf/api/rpc_test1.pb.h"
#include "casock/util/Logger.h"

class test1_cxx : public CxxTest::TestSuite
{
  private:
    tests::rpc::asio::protobuf::Test1ServiceImpl mServiceServer;
    casock::proactor::asio::base::AsyncProcessor* mpAsyncProcessor;

  public:
    void setUp ()
    {
      LOGGER->setDebugLevel (MAX_LEVEL);
      casock::proactor::asio::base::AsyncProcessor::initialize ();
      mpAsyncProcessor = casock::proactor::asio::base::AsyncProcessor::getInstance ();
    }

    void tearDown ()
    {
      casock::proactor::asio::base::AsyncProcessor::destroy ();
      LOGGER->finalize ();
    }

  public:
    void test_basic ()
    {
      /*! server */
      casock::rpc::asio::protobuf::server::RPCServerProxy serverProxy (*mpAsyncProcessor, 2000, &mServiceServer);
      mServiceServer.setProxy (&serverProxy);
      serverProxy.start ();

      /*! client */
      casock::rpc::asio::protobuf::client::RPCClientProxy clientProxy (*mpAsyncProcessor, "localhost", "2000");
      tests::rpc::protobuf::api::Test1Service* pServiceClient = new tests::rpc::protobuf::api::Test1Service::Stub (&clientProxy);

      /*! sending message */
      tests::rpc::protobuf::api::Test1Request* request = new tests::rpc::protobuf::api::Test1Request ();
      tests::rpc::protobuf::api::Test1Response* response = new tests::rpc::protobuf::api::Test1Response ();
      casock::rpc::protobuf::client::RPCCallController* controller = new casock::rpc::protobuf::client::RPCCallController ();

      request->set_id (1);
      request->set_message (2);
      tests::rpc::asio::protobuf::Test1ResponseHandlerImpl handler (controller, response);
      pServiceClient->Test1Call (controller, request, response, handler.closure ());

      /*! running async processor */
      mpAsyncProcessor->run ();

      TS_ASSERT_EQUALS (request->id (), response->id ());
      TS_ASSERT_EQUALS (request->message (), response->message ());
    }
};

#endif // __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST1_CXX_H_
