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

#include "casock/rpc/asio/protobuf/server/RPCSocketSession.h"

#include <boost/bind.hpp>

#include "casock/rpc/protobuf/server/RPCCall.h"
#include "casock/rpc/protobuf/server/RPCCallQueue.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/util/Lock.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace server {
          RPCSocketSession::RPCSocketSession (
              casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor,
              casock::proactor::asio::server::SocketServer& rSocketServer,
              RPCCallQueue<RPCCallResponseHandler>& rCallQueue)
            : casock::proactor::asio::server::SocketSession (rAsyncProcessor, rSocketServer), mCommunicator (this), mrCallQueue (rCallQueue)
          { }

          void RPCSocketSession::onConnect ()
          {
            LOGMSG (LOW_LEVEL, "RPCSocketSession::%s ()\n", __FUNCTION__);

            try
            {
              mCommunicator.recvRequest (::boost::bind (&RPCSocketSession::onRecvRequest, this, ::asio::placeholders::error, _2));
            }
            catch (casock::base::CASClosedConnectionException& e)
            {
              LOGMSG (NO_DEBUG, "RPCSocketSession::%s () - catch (casock::base::CASClosedConnectionException&) [%s]\n", __FUNCTION__, e.what ());
            }
          }

          void RPCSocketSession::onRecvRequest (const ::asio::error_code& error, ::google::protobuf::Message* pMessage)
          {
            LOGMSG (LOW_LEVEL, "RPCSocketSession::%s () - error [%d], pMessage [%p]\n", __FUNCTION__, error.value (), pMessage);

            if (! error)
            {
              casock::rpc::protobuf::api::RpcRequest* request = static_cast<casock::rpc::protobuf::api::RpcRequest *> (pMessage);

              LOGMSG (HIGH_LEVEL, "RPCSocketSession::%s () - request received: %d bytes - id [%u], operation [%s]\n", __FUNCTION__, request->ByteSize (), request->id (), request->operation ().c_str ());

              /*!
               * The RPCCall<RPCCallResponseHandler> will be deleted by RPCCallEntry destructor.
               * The RPCCallEntry will be created by RPCCallHandler<_TpResponseHandler>::run () 
               *  and deleted on RPCCallHandler<_TpResponseHandler>::callback (RPCCallEntry*).
               */
              mrCallQueue.push (new casock::rpc::protobuf::server::RPCCall<RPCCallResponseHandler> (this, request));

              try
              {
                mCommunicator.recvRequest (::boost::bind (&RPCSocketSession::onRecvRequest, this, ::asio::placeholders::error, _2));
              }
              catch (casock::base::CASClosedConnectionException& e)
              {
                LOGMSG (NO_DEBUG, "RPCSocketSession::%s () - catch (casock::base::CASClosedConnectionException&) [%s]\n", __FUNCTION__, e.what ());
                invalidateCalls ();
              }
            }
            else
            {
              invalidateCalls ();
            }
          }

          void RPCSocketSession::onSentResponse (const ::asio::error_code& error)
          {
            LOGMSG (LOW_LEVEL, "RPCSocketSession::%s () - error [%d]\n", __FUNCTION__, error.value ());
          }

          void RPCSocketSession::callback (const casock::rpc::protobuf::api::RpcResponse& response)
          {
            LOGMSG (LOW_LEVEL, "RPCSocketSession::%s ()\n", __FUNCTION__);

            try
            {
              mCommunicator.sendResponse (response, ::boost::bind (&RPCSocketSession::onSentResponse, this, ::asio::placeholders::error));
            }
            catch (casock::base::CASClosedConnectionException& e)
            {
              LOGMSG (NO_DEBUG, "RPCSocketSession::%s () - catch (casock::base::CASClosedConnectionException&) [%s]\n", __FUNCTION__, e.what ());
            }
          }
        }
      }
    }
  }
}
