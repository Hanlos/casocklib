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
 * \file tests/rpc/asio/protobuf/Test2ClientThread.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST2_CLIENT_THREAD_H_
#define __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST2_CLIENT_THREAD_H_

#include "casock/util/Thread.h"

namespace tests {
  namespace rpc {
    namespace protobuf {
      namespace api {
        class TestService;
      }
    }

    namespace asio {
      namespace protobuf {
        class Test2Manager;

        class Test2ClientThread : public casock::util::Thread
        {
          public:
            Test2ClientThread ();

          public:
            void setNumCalls (const uint32& calls) { mNumCalls = calls; }
            void setService (tests::rpc::protobuf::api::TestService* pService) { mpService = pService; }
            void setManager (Test2Manager* pManager) { mpManager = pManager; }

          public:
            void run ();

          private:
            uint32 mNumCalls;
            tests::rpc::protobuf::api::TestService* mpService;
            Test2Manager* mpManager;
        };
      }
    }
  }
}

#endif // __CASOCKLIB__TESTS_RPC_ASIO_PROTOBUF__TEST2_CLIENT_THREAD_H_