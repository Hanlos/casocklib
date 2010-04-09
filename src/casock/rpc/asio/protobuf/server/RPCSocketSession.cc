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
 * \file casock/rpc/asio/protobuf/server/RPCSocketSession.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include <boost/bind.hpp>

#include "casock/rpc/asio/protobuf/server/RPCSocketSession.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace server {
          RPCSocketSession::RPCSocketSession (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, RPCCallQueue<RPCCallResponseHandler>& rCallQueue)
            : casock::proactor::asio::server::SocketSession (rAsyncProcessor), mrCallQueue (rCallQueue)
          { }

          void RPCSocketSession::onConnect ()
          {
            LOGMSG (LOW_LEVEL, "RPCSocketSession::%s ()\n", __FUNCTION__);
            readsome (buffer.buff, buffer.size, ::boost::bind (&RPCSocketSession::onReadBuffer, this, ::asio::placeholders::error, ::asio::placeholders::bytes_transferred));
          }

          void RPCSocketSession::onReadBuffer (const ::asio::error_code& error, const size_t& bytes_transferred)
          {
            LOGMSG (LOW_LEVEL, "RPCSocketSession::%s () - bytes_transferred [%zu], buffer [%s]\n", __FUNCTION__, bytes_transferred, buffer.buff);
            // TODO: create a RpcRequest with the buffer and put it in the queue
            readsome (buffer.buff, buffer.size, ::boost::bind (&RPCSocketSession::onReadBuffer, this, ::asio::placeholders::error, ::asio::placeholders::bytes_transferred));
          }

          void RPCSocketSession::callback (const RpcResponse* const pResponse)
          {
            // TODO: asynchronously write response
          }
        }
      }
    }
  }
}
