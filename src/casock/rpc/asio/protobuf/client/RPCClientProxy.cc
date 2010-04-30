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
#include "casock/rpc/protobuf/client/RPCCall.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"
#include "casock/rpc/protobuf/client/RPCCallHandler.h"
#include "casock/rpc/asio/protobuf/client/RPCSocketClient.h"
#include "casock/rpc/asio/protobuf/client/RPCChannel.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          RPCClientProxy::RPCClientProxy (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port)
          {
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::RPCClientProxy ()\n");

            //mpRPCSocketClient = new RPCSocketClient (rAsyncProcessor, host, port, *this, mCallHash, *mpCallQueue);
            mpRPCSocketClient = new RPCSocketClient (rAsyncProcessor, host, port, mCallHash, *mpCallQueue);
            mpChannel = new RPCChannel (mpRPCSocketClient->communicator ());
            //mpService = new casock::rpc::protobuf::api::RpcService::Stub (mpChannel);
          }

          void RPCClientProxy::sendRpcRequest (const casock::rpc::protobuf::api::RpcRequest& request, casock::rpc::protobuf::client::RPCCall* pCall)
          {
            mpChannel->RpcCall (request, ::boost::bind (&RPCClientProxy::onSentRequest, this, _1, request.id (), pCall));
          }

					void RPCClientProxy::onSentRequest (const ::asio::error_code& error, const uint32 id, casock::rpc::protobuf::client::RPCCall* pCall)
					{
						if (! error)
						{
							registerRPCCall (id, pCall);
						}
						else
						{
              pCall->controller ()->SetFailed ("Impossible to send request [" + error.message () + "]");
              mpCallQueue->push (pCall);
						}
					}
        }
      }
    }
  }
}
