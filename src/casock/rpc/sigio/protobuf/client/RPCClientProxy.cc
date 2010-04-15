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
 * \file casock/rpc/sigio/protobuf/client/RPCClientProxy.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "RPCClientProxy.h"

#include <sstream>
using std::stringstream;

#include <google/protobuf/message.h>
#include <google/protobuf/descriptor.h>

#include "casock/util/Logger.h"
#include "casock/sigio/client/ClientSocket.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"
//#include "casock/rpc/protobuf/client/RPCCall.h"
//#include "casock/rpc/protobuf/client/RPCCallQueue.h"
//#include "casock/rpc/protobuf/client/RPCCallHandler.h"
//#include "casock/rpc/sigio/protobuf/client/RPCCallController.h"
#include "casock/rpc/sigio/protobuf/client/RPCChannel.h"
#include "casock/rpc/sigio/protobuf/client/RPCReaderHandler.h"

namespace casock {
  namespace rpc {
    namespace sigio {
      namespace protobuf {
        namespace client {
          RPCClientProxy::RPCClientProxy (casock::sigio::base::Dispatcher& rDispatcher, const string& host, const unsigned int& port)
          {
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::RPCClientProxy ()\n");

            mpClientSocket = new ClientSocket (rDispatcher, host, port);
            mpClientSocket->connect ();
            mpReaderHandler = new RPCReaderHandler (rDispatcher, mpClientSocket, this, mCallHash, *mpCallQueue);
            mpChannel = new RPCChannel (&(mpReaderHandler->communicator ()));
            mpService = new casock::rpc::protobuf::api::RpcService::Stub (mpChannel);
          }

          void RPCClientProxy::sendRpcRequest (casock::rpc::protobuf::api::RpcRequest* pRequest)
          {
            mpService->RpcCall (NULL, pRequest, NULL, NULL);
          }

          /*
          void RPCClientProxy::CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done)
          {
            LOGMSG (HIGH_LEVEL, "RPCClientProxy::%s ()\n", __FUNCTION__);

            if (mpClientSocket->connected ())
            {
              casock::rpc::protobuf::api::RpcRequest* pRpcRequest = new casock::rpc::protobuf::api::RpcRequest ();

              pRpcRequest->set_id (1);
              pRpcRequest->set_operation (method->name ());
              pRpcRequest->set_request (request->SerializeAsString ());

              mCallHash.lock ();
              mCallHash [pRpcRequest->id ()] = new RPCCall (response, controller, done);
              mCallHash.unlock ();

              mpService->RpcCall (NULL, pRpcRequest, NULL, NULL);
            }
            else
              LOGMSG (HIGH_LEVEL, "RPCClientProxy::%s () - socket disconnected!\n", __FUNCTION__);
          }
          */
        }
      }
    }
  }
}
