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
 * \file casock/rpc/asio/protobuf/server/RPCServerProxy.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/server/RPCServerProxy.h"

#include "casock/util/types.h"
#include "casock/util/Logger.h"
#include "casock/rpc/protobuf/server/RPCCallQueue.h"
#include "casock/rpc/protobuf/server/RPCCallHandler.h"
#include "casock/rpc/protobuf/server/RPCCallResponseHandler.h"
#include "casock/rpc/asio/protobuf/server/RPCSocketServer.h"
#include "casock/rpc/asio/protobuf/server/RPCSocketServerFactory.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace server {
          RPCServerProxy::RPCServerProxy (
              const RPCSocketServerFactory& rSocketServerFactory,
              const RPCCallHandlerFactory& rCallHandlerFactory)
            : casock::rpc::protobuf::server::RPCServerProxy (rCallHandlerFactory)
          {
            LOGMSG (HIGH_LEVEL, "%s\n", __PRETTY_FUNCTION__);

            //mpSocketServer = new casock::rpc::asio::protobuf::server::RPCSocketServerImpl (rAsyncProcessor, port, *mpCallQueue);
            mpSocketServer = rSocketServerFactory.buildRPCSocketServer (*mpCallQueue);
          }

          RPCServerProxy::~RPCServerProxy ()
          {
            //if (m_running)
            //  stop ();

            delete mpSocketServer;
          }

          void RPCServerProxy::start ()
          {
            mpSocketServer->start ();

            //m_running = true;
          }

          void RPCServerProxy::stop ()
          {
            mpSocketServer->close ();

            //m_running = false;
          }

          void RPCServerProxy::stopReceivingCalls ()
          {
            LOGMSG (LOW_LEVEL, "RPCServerProxy::%s ()\n", __FUNCTION__);
            mpSocketServer->closeAcceptor ();
          }
        }
      }
    }
  }
}
