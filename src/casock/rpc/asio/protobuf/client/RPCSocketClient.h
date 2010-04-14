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
 * \file casock/rpc/asio/protobuf/client/RPCSocketClient.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_SOCKET_CLIENT_H_
#define __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_SOCKET_CLIENT_H_

#include <boost/bind.hpp>

#include "casock/util/Logger.h"
#include "casock/util/LockableHash.h"
#include "casock/proactor/asio/client/SocketClient.h"
#include "casock/rpc/asio/protobuf/client/RPCClientCommunicator.h"

namespace casock {
  namespace rpc {
    namespace protobuf {
      namespace client {
        class RPCCall;
        class RPCCallQueue;
      }
    }

    namespace asio {
      namespace protobuf {
        namespace client {
          class RPCClientProxy;

          using casock::util::LockableHash;
          using casock::rpc::protobuf::client::RPCCall;
          using casock::rpc::protobuf::client::RPCCallQueue;

          class RPCSocketClient : public casock::proactor::asio::client::SocketClient
          {
            public:
              RPCSocketClient (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const std::string& port, RPCClientProxy& rClientProxy, LockableHash<uint32, RPCCall*>& rCallHash, RPCCallQueue& rCallQueue);

            protected:
              void onConnect ();
              void onConnectionFailure ();

            public:
              RPCClientCommunicator& communicator () { return mCommunicator; }

              /*

                 void onSentBuffer (const ::asio::error_code& error)
                 {
                 if (! error)
                 {
                 LOGMSG (LOW_LEVEL, "RPCSocketClient::%s () - NO ERROR!\n", __FUNCTION__);
                 mCommunicator.readsome (mBuffer.buff, mBuffer.size, ::boost::bind (&RPCSocketClient::onReadBuffer, this, ::asio::placeholders::error, ::asio::placeholders::bytes_transferred));
                 }
                 else
                 {
                 LOGMSG (LOW_LEVEL, "RPCSocketClient::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
                 close ();
                 }
                 }

                 void onReadBuffer (const ::asio::error_code& error, const size_t& bytes_transferred)
                 {
                 LOGMSG (LOW_LEVEL, "RPCSocketClient::%s () - bytes_transferred [%zu], mBuffer [%s]\n", __FUNCTION__, bytes_transferred, mBuffer.buff);
                 close ();
                 }
               */

            private:
              RPCClientCommunicator           mCommunicator;
              RPCClientProxy&                 mrClientProxy;
              LockableHash<uint32, RPCCall*>& mrCallHash;
              RPCCallQueue&                   mrCallQueue;

          };
        }
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_ASIO_PROTOBUF_CLIENT__RPC_SOCKET_CLIENT_H_
