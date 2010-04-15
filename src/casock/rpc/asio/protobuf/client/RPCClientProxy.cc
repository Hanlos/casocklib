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
 * \file casock/rpc/asio/protobuf/client/RPCClientProxy.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/client/RPCClientProxy.h"

#include <google/protobuf/descriptor.h>

#include "casock/util/Logger.h"
#include "casock/rpc/asio/protobuf/client/RPCSocketClient.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"
#include "casock/rpc/protobuf/client/RPCCallHandler.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          RPCClientProxy::RPCClientProxy (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port)
          {
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::RPCClientProxy ()\n");

            mpRPCSocketClient = new RPCSocketClient (rAsyncProcessor, host, port, *this, mCallHash, *mpCallQueue);
            mpChannel = new RPCChannel (mpRPCSocketClient->communicator ());
            mpService = new casock::rpc::protobuf::api::RpcService::Stub (mpChannel);
          }

          void RPCClientProxy::sendRpcRequest (casock::rpc::protobuf::api::RpcRequest* pRequest)
          {
            mpService->RpcCall (NULL, pRequest, NULL, NULL);
          }
        }
      }
    }
  }
}
