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
 * \file casock/rpc/asio/protobuf/server/RPCCommunicator.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_SERVER__RPC_SERVER_COMMUNICATOR_H_
#define __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_SERVER__RPC_SERVER_COMMUNICATOR_H_

#include "casock/rpc/asio/protobuf/base/RPCCommunicator.h"

namespace casock {
  namespace rpc {
    namespace protobuf {
      namespace api {
        class RpcRequest;
      }
    }

    namespace asio {
      namespace protobuf {
        namespace server {
          using casock::proactor::asio::base::SocketChannel;

          class RPCServerCommunicator : public casock::rpc::asio::protobuf::base::RPCCommunicator
          {
            public:
              RPCServerCommunicator (SocketChannel* const pChannel);

            private:
              ::google::protobuf::Message* createRecvMessage ();

            public:
              void recvRequest (::boost::function<void(const ::asio::error_code&, ::google::protobuf::Message*)> handler);
              void sendResponse (const ::google::protobuf::Message* const message, ::boost::function<void(const ::asio::error_code&)> handler);

//            private:
//              size_t mSize;
          };
        }
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_SERVER__RPC_SERVER_COMMUNICATOR_H_
