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
 * \file tests/rpc/asio/protobuf/test2_cxx.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */


#ifndef __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST2_CXX_H_
#define __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST2_CXX_H_

#include <cxxtest/TestSuite.h>
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/asio/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/asio/protobuf/server/RPCServerProxy.h"
#include "tests/rpc/asio/protobuf/Test1ServiceImpl.h"
#include "tests/rpc/asio/protobuf/Test1ResponseHandlerImpl.h"
#include "tests/rpc/protobuf/api/rpc_test.pb.h"
#include "casock/util/Logger.h"

class test2_cxx : public CxxTest::TestSuite
{
  public:
    void setUp ()
    {
      LOGGER->setDebugLevel (MAX_LEVEL);
    }

    void tearDown ()
    {
      LOGGER->finalize ();
    }

  public:
    void test_basic ()
    {
      TS_ASSERT_EQUALS (1, 1);
    }
};

#endif // __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST2_CXX_H_
