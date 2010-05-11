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
 * \file casock/rpc/asio/protobuf/client/RPCClientCommunicator.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/client/RPCClientCommunicator.h"

#include "casock/util/Logger.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          RPCClientCommunicator::RPCClientCommunicator (SocketChannel* const pChannel)
            : casock::rpc::asio::protobuf::base::RPCCommunicator (pChannel)
          { }

          ::google::protobuf::Message* RPCClientCommunicator::createRecvMessage ()
          {
            return new casock::rpc::protobuf::api::RpcResponse ();
          }

          void RPCClientCommunicator::sendRequest (const ::google::protobuf::Message& message, ::boost::function<void(const ::asio::error_code&)> handler)
          {
            LOGMSG (NO_DEBUG, "RPCClientCommunicator::%s () - sending size [%zd]\n", __FUNCTION__, message.ByteSize ());
            sendMessage (message, handler);
          }

          void RPCClientCommunicator::recvResponse (::boost::function<void(const ::asio::error_code&, ::google::protobuf::Message*)> handler)
          {
            LOGMSG (NO_DEBUG, "RPCClientCommunicator::%s ()\n", __FUNCTION__);
            recvMessage (handler);
          }
        }
      }
    }
  }
}
