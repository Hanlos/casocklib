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
 * \file casock/rpc/asio/protobuf/client/RPCChannel.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/client/RPCChannel.h"

#include <boost/bind.hpp>
#include <google/protobuf/message.h>
#include "casock/rpc/asio/protobuf/client/RPCClientCommunicator.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/util/Logger.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          RPCChannel::RPCChannel (RPCClientCommunicator& rCommunicator)
            : mrCommunicator (rCommunicator)
          { }

          void RPCChannel::onSentRequest (const ::asio::error_code& error)
          {
            LOGMSG (HIGH_LEVEL, "RPCChannel::%s ()\n", __FUNCTION__);

            if (error)
            {
              /*!
               * TODO:
               *
               * \todo If we got an error before send the request we need recover the
               * RPCCall from the RPCCallHash, set the error and put it on the RPCCallQueue.
               *
               * Another solution is to create the RPCCall here. For do this we need to get
               * access to the pointers to: response, controller and closure. This way, we'd
               * have the following situations:
               * If the request was successfully sent, we need to put RPCCall on RPCCallHash.
               * If the request was not sent, we need to indicate the error in the controller
               * and put RPCCall on the RPCCallQueue.
               */
            }
          }

          void RPCChannel::CallMethod(const google::protobuf::MethodDescriptor*, google::protobuf::RpcController*, const google::protobuf::Message* request, google::protobuf::Message*, google::protobuf::Closure*)
          {
            LOGMSG (HIGH_LEVEL, "RPCChannel::%s ()\n", __FUNCTION__);

            mrCommunicator.sendRequest (request, ::boost::bind (&RPCChannel::onSentRequest, this, ::asio::placeholders::error));
          }

          /*!
           * TODO:
           * \todo We need an onSentRequest method that receives a pRPCCall as parameter.
           * If we got some error, pRPCCall->closure () is executed. If not, pRPCCall is
           * put into mrCallHash.
           */
				  void RPCChannel::RpcCall (const casock::rpc::protobuf::api::RpcRequest& request, ::boost::function<void(const ::asio::error_code&)> handler) //, const uint32, casock::rpc::protobuf::client::RPCCall*)> handler)
          {
            //mrCommunicator.sendRequest (&request, handler);

            /*!
             * TODO
             * \todo We need to use pRPCCall.
             */
          }
        }
      }
    }
  }
}
