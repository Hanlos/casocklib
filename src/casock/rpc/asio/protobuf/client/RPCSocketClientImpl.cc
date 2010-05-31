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
 * \file casock/rpc/asio/protobuf/client/RPCSocketClientImpl.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/client/RPCSocketClientImpl.h"

#include <boost/bind.hpp>

#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/rpc/protobuf/client/RPCCall.h"
#include "casock/rpc/protobuf/client/RPCCallHash.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace client {
          RPCSocketClientImpl::RPCSocketClientImpl (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port, RPCCallHash& rCallHash, RPCCallQueue& rCallQueue)
            : RPCSocketClient (rAsyncProcessor, host, port), mCommunicator (this), mrCallHash (rCallHash), mrCallQueue (rCallQueue)
          {
            LOGMSG (LOW_LEVEL, "%s - host [%s], port [%s]\n", __PRETTY_FUNCTION__, host.c_str (), port.c_str ());
            syncConnect ();
//            onConnect ();
          }

          void RPCSocketClientImpl::onConnect ()
          {
            LOGMSG (LOW_LEVEL, "RPCSocketClientImpl::%s ()\n", __FUNCTION__);

            try
            {
              mCommunicator.recvResponse (::boost::bind (&RPCSocketClientImpl::onRecvResponse, this, ::asio::placeholders::error, _2));
            }
            catch (casock::base::CASClosedConnectionException& e)
            {
              LOGMSG (NO_DEBUG, "RPCSocketSession::%s () - catch (casock::base::CASClosedConnectionException&) [%s]\n", __FUNCTION__, e.what ());
            }
          }

          void RPCSocketClientImpl::onConnectionFailure ()
          {
            LOGMSG (LOW_LEVEL, "RPCSocketClientImpl::%s ()\n", __FUNCTION__);
          }

          void RPCSocketClientImpl::onRecvResponse (const ::asio::error_code& error, ::google::protobuf::Message* pMessage)
          {
            LOGMSG (NO_DEBUG, "RPCSocketClientImpl::%s ()\n", __FUNCTION__);

            if (! error)
            {
              LOGMSG (LOW_LEVEL, "RPCSocketClientImpl::%s () - no error!\n", __FUNCTION__);

              casock::rpc::protobuf::api::RpcResponse* pResponse = static_cast<casock::rpc::protobuf::api::RpcResponse *>(pMessage);

              LOGMSG (LOW_LEVEL, "RPCSocketClientImpl::%s () - response received: %d bytes - id [%u]\n", __FUNCTION__, pResponse->ByteSize (), pResponse->id ());

              casock::rpc::protobuf::client::RPCCall* pCall = mrCallHash.pop (pResponse->id ());

              if (pCall)
              {
                pCall->setRpcResponse (pResponse);
                mrCallQueue.push (pCall);
              }
              else
              {
                LOGMSG (NO_DEBUG, "%s - pCall not found in mrCallHash for ID [%u]\n", __PRETTY_FUNCTION__, pResponse->id ());
                delete pResponse;
              }

              try
              {
                mCommunicator.recvResponse (::boost::bind (&RPCSocketClientImpl::onRecvResponse, this, ::asio::placeholders::error, _2));
              }
              catch (casock::base::CASClosedConnectionException& e)
              {
                LOGMSG (NO_DEBUG, "RPCSocketSession::%s () - catch (casock::base::CASClosedConnectionException&) [%s]\n", __FUNCTION__, e.what ());
              }
            }
            else
            {
              LOGMSG (LOW_LEVEL, "RPCSocketClientImpl::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
            }
          }
        }
      }
    }
  }
}
