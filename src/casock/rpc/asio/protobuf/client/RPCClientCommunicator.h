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
 * \file casock/rpc/asio/protobuf/client/RPCClientCommunicator.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_CLIENT_COMMUNICATOR_H_
#define __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_CLIENT_COMMUNICATOR_H_

#include "casock/rpc/asio/protobuf/base/RPCCommunicator.h"

namespace casock {
  namespace rpc {

    namespace asio {
      namespace protobuf {
        namespace client {
          using casock::proactor::asio::base::SocketChannel;

          class RPCClientCommunicator : public casock::rpc::asio::protobuf::base::RPCCommunicator
          {
            public:
              RPCClientCommunicator (SocketChannel* const pChannel);

            private:
              ::google::protobuf::Message* createRecvMessage ();

            public:
              void sendRequest (const ::google::protobuf::Message* const message, ::boost::function<void(const ::asio::error_code&)> handler);
              void recvResponse (::boost::function<void(const ::asio::error_code&, ::google::protobuf::Message*)> handler);

            private:
              size_t mSize;
          };
        }
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_CLIENT_COMMUNICATOR_H_
