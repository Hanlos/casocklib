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
 * \file casock/rpc/asio/protobuf/client/RPCClientProxy.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_CLIENT_PROXY_H_
#define __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_CLIENT_PROXY_H_

#include <string>
#include "casock/rpc/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/asio/protobuf/client/RPCChannel.h"

namespace casock {
  namespace proactor {
    namespace asio {
      namespace base {
        class AsyncProcessor;
      }
    }
  }

  namespace rpc {
    namespace protobuf {
      namespace api {
        class RpcService;
        class RpcRequest;
      }
    }

    namespace asio {
      namespace protobuf {
        namespace client {
          class RPCChannel;
          class RPCSocketClient;

          using casock::rpc::protobuf::api::RpcService;

          class RPCClientProxy : public casock::rpc::protobuf::client::RPCClientProxy
          {
            public:
              RPCClientProxy (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port);

            private:
              void sendRpcRequest (casock::rpc::protobuf::api::RpcRequest* pRequest);

            private:
              RPCSocketClient*  mpRPCSocketClient;
              RPCChannel*       mpChannel;
              RpcService*       mpService;
          };
        }
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_CLIENT_PROXY_H_
