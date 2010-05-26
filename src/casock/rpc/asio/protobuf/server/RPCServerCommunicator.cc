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
 * \file casock/rpc/asio/protobuf/server/RPCServerCommunicator.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/server/RPCServerCommunicator.h"

#include <boost/bind.hpp>

#include "casock/rpc/protobuf/api/rpc.pb.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace server {
          RPCServerCommunicator::RPCServerCommunicator (SocketChannel* const pChannel)
            : casock::rpc::asio::protobuf::base::RPCCommunicator (pChannel)
          { }

          ::google::protobuf::Message* RPCServerCommunicator::createRecvMessage ()
          {
            LOGMSG (MEDIUM_LEVEL, "RPCServerCommunicator::%s ()\n", __FUNCTION__);
            return new casock::rpc::protobuf::api::RpcRequest ();
          }

          void RPCServerCommunicator::recvRequest (::boost::function<void(const ::asio::error_code&, ::google::protobuf::Message*)> handler)
          {
            LOGMSG (MEDIUM_LEVEL, "RPCServerCommunicator::%s ()\n", __FUNCTION__);
            recvMessage (handler);
          }

          void RPCServerCommunicator::sendResponse (const ::google::protobuf::Message& message, ::boost::function<void(const ::asio::error_code&)> handler)
          {
            LOGMSG (MEDIUM_LEVEL, "RPCServerCommunicator::%s ()\n", __FUNCTION__);
            sendMessage (message, handler);
          }
        }
      }
    }
  }
}


