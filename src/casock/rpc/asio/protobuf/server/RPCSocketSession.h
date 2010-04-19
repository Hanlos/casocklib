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
 * \file casock/rpc/asio/protobuf/server/RPCSocketSession.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_SERVER__RPC_SOCKET_SESSION_H_
#define __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_SERVER__RPC_SOCKET_SESSION_H_

#include "casock/proactor/asio/server/SocketSession.h"
#include "casock/rpc/protobuf/server/RPCCallResponseHandler.h"
#include "casock/rpc/asio/protobuf/server/RPCServerCommunicator.h"

namespace casock {
  namespace util {
    class Lock;
  }

  namespace rpc {
    namespace protobuf {
      namespace api {
        class RpcResponse;
      }

      namespace server {
        template<typename _TpResponseHandler>
          class RPCCallQueue;
      }
    }

    namespace asio {
      namespace protobuf {
        namespace server {
          using casock::rpc::protobuf::api::RpcResponse;
          using casock::rpc::protobuf::server::RPCCallQueue;
          using casock::rpc::protobuf::server::RPCCallResponseHandler;

          class RPCSocketSession : public casock::proactor::asio::server::SocketSession, public RPCCallResponseHandler
          {
            public:
              RPCSocketSession (
                  casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor,
                  casock::proactor::asio::server::SocketServer& rSocketServer,
                  RPCCallQueue<RPCCallResponseHandler>& rCallQueue);

            private:
              void onConnect ();
              void onRecvRequest (const ::asio::error_code& error, ::google::protobuf::Message* pMessage);
              void onSentResponse (const ::asio::error_code& error, casock::util::Lock* pLock);

            public:
              void callback (const RpcResponse& response);

            private:
              RPCServerCommunicator                 mCommunicator;
              RPCCallQueue<RPCCallResponseHandler>& mrCallQueue;
          };
        }
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_SERVER__RPC_SOCKET_SESSION_H_
