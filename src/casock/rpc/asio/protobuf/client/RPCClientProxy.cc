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

#include <boost/bind.hpp>
#include <google/protobuf/descriptor.h>

#include "casock/util/Logger.h"
#include "casock/rpc/protobuf/client/RPCCall.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"
#include "casock/rpc/protobuf/client/RPCCallHandler.h"
#include "casock/rpc/asio/protobuf/client/RPCSocketClientImpl.h"
#include "casock/rpc/asio/protobuf/client/RPCSocketClientFactory.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          /*
          RPCClientProxy::RPCClientProxy (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port)
          {
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::RPCClientProxy ()\n");

            mpRPCSocketClient = new RPCSocketClientImpl (rAsyncProcessor, host, port, mCallHash, *mpCallQueue);
          }
          */

          RPCClientProxy::RPCClientProxy (RPCSocketClientFactory* pSocketClientFactory, RPCCallHandlerFactory* pCallHandlerFactory)
            : casock::rpc::protobuf::client::RPCClientProxy (pCallHandlerFactory)
          {
            LOGMSG (HIGH_LEVEL, "%s\n", __PRETTY_FUNCTION__);
            mpRPCSocketClient = pSocketClientFactory->buildRPCSocketClient (mCallHash, *mpCallQueue);
          }

          RPCClientProxy::~RPCClientProxy ()
          {
            delete mpRPCSocketClient;
          }

          void RPCClientProxy::sendRpcRequest (const casock::rpc::protobuf::api::RpcRequest& request, casock::rpc::protobuf::client::RPCCall* pCall)
          {
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::%s ()\n", __FUNCTION__);
            mpRPCSocketClient->communicator ().sendRequest (request, ::boost::bind (&RPCClientProxy::onSentRequest, this, _1, request.id (), pCall));
          }

					void RPCClientProxy::onSentRequest (const ::asio::error_code& error, const uint32 id, casock::rpc::protobuf::client::RPCCall* pCall)
					{
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::%s ()\n", __FUNCTION__);

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

          void RPCClientProxy::close ()
          {
            mpRPCSocketClient->close ();
          }
        }
      }
    }
  }
}
