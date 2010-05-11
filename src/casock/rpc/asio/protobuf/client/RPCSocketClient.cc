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
 * \file casock/rpc/asio/protobuf/client/RPCSocketClient.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/client/RPCSocketClient.h"

#include <boost/bind.hpp>

//#include "casock/rpc/asio/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/protobuf/client/RPCCall.h"
#include "casock/rpc/protobuf/client/RPCCallHash.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          //RPCSocketClient::RPCSocketClient (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port, RPCClientProxy& rClientProxy, RPCCallHash& rCallHash, RPCCallQueue& rCallQueue)
          //  : casock::proactor::asio::client::SocketClient (rAsyncProcessor, host, port), mCommunicator (this), mrClientProxy (rClientProxy), mrCallHash (rCallHash), mrCallQueue (rCallQueue)
          RPCSocketClient::RPCSocketClient (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port, RPCCallHash& rCallHash, RPCCallQueue& rCallQueue)
            : casock::proactor::asio::client::SocketClient (rAsyncProcessor, host, port), mCommunicator (this), mrCallHash (rCallHash), mrCallQueue (rCallQueue)
          {
            LOGMSG (LOW_LEVEL, "RPCSocketClient::RPCSocketClient () - host [%s], port [%s]\n", host.c_str (), port.c_str ());

            syncConnect ();
            onConnect ();
          }

          void RPCSocketClient::onConnect ()
          {
            LOGMSG (LOW_LEVEL, "RPCReaderHandler::%s ()\n", __FUNCTION__);

            mCommunicator.recvResponse (::boost::bind (&RPCSocketClient::onRecvResponse, this, ::asio::placeholders::error, _2));
          }

          void RPCSocketClient::onConnectionFailure ()
          {
            LOGMSG (LOW_LEVEL, "RPCSocketClient::%s ()\n", __FUNCTION__);
          }

          void RPCSocketClient::onRecvResponse (const ::asio::error_code& error, ::google::protobuf::Message* pMessage)
          {
            LOGMSG (MAX_LEVEL, "RPCSocketClient::%s ()\n", __FUNCTION__);

            if (! error)
            {
              LOGMSG (LOW_LEVEL, "RPCReaderHandler::%s () - no error!\n", __FUNCTION__);

              casock::rpc::protobuf::api::RpcResponse* pResponse = static_cast<casock::rpc::protobuf::api::RpcResponse *>(pMessage);

              LOGMSG (LOW_LEVEL, "RPCReaderHandler::%s () - response received: %d bytes - id [%u]\n", __FUNCTION__, pResponse->ByteSize (), pResponse->id ());

              RPCCall* pCall = mrCallHash.pop (pResponse->id ());

              if (pCall)
              {
                pCall->setRpcResponse (pResponse);
                mrCallQueue.push (pCall);
              }

              mCommunicator.recvResponse (::boost::bind (&RPCSocketClient::onRecvResponse, this, ::asio::placeholders::error, _2));
            }
            else
            {
              LOGMSG (LOW_LEVEL, "RPCReaderHandler::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
            }
          }
        }
      }
    }
  }
}
