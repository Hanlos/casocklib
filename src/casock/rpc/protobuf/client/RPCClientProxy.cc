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
 * \file casock/rpc/protobuf/client/RPCClientProxy.cc
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
#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/rpc/protobuf/client/RPCCall.h"
#include "casock/rpc/protobuf/client/RPCCallQueue.h"
#include "casock/rpc/protobuf/client/RPCCallHandler.h"
#include "casock/rpc/protobuf/client/RPCCallHandlerFactory.h"
#include "casock/rpc/protobuf/client/RPCRequestBuilder.h"

namespace casock {
  namespace rpc {
    namespace protobuf {
      namespace client {
        //uint32 RPCClientProxy::mID = 0;
        uint32 RPCClientProxy::DEFAULT_NUM_CALL_HANDLERS = 1;

        RPCClientProxy::RPCClientProxy (const RPCCallHandlerFactory& rCallHandlerFactory, const uint32& numCallHandlers)
          : mrCallHandlerFactory (rCallHandlerFactory)
        {
          LOGMSG (HIGH_LEVEL, "RPCClientProxy::RPCClientProxy ()\n");

          mpCallQueue = new RPCCallQueue ();
          mpRequestBuilder = new RPCRequestBuilder ();

          setNumCallHandlers (numCallHandlers);
        }

        RPCClientProxy::~RPCClientProxy ()
        {
          /*!
           * The call handlers are threads waiting for messages in the call queue.
           * Before we delete the call queue we need to cancel the call handlers.
           */
          LOGMSG (LOW_LEVEL, "%s - remove call handlers...\n", __PRETTY_FUNCTION__);
          removeCallHandlers (mCallHandlers.size ());

          LOGMSG (LOW_LEVEL, "%s - delete mpCallQueue [%zp]...\n", __PRETTY_FUNCTION__, mpCallQueue);
          delete mpCallQueue;

          LOGMSG (LOW_LEVEL, "%s\n - end", __PRETTY_FUNCTION__);
        }

        void RPCClientProxy::addCallHandlers (const uint32& n)
        {
          LOGMSG (MEDIUM_LEVEL, "%s - n [%zu]\n", __PRETTY_FUNCTION__, n);

          for (uint32 i = 0; i < n; i++)
          {
            LOGMSG (HIGH_LEVEL, "%s - i [%zu] - build call handler\n", __PRETTY_FUNCTION__, i);
            RPCCallHandler* pCallHandler = mrCallHandlerFactory.buildRPCCallHandler (*mpCallQueue);

            LOGMSG (HIGH_LEVEL, "%s - i [%zu] - start call handler\n", __PRETTY_FUNCTION__, i);
            pCallHandler->start ();

            LOGMSG (HIGH_LEVEL, "%s - i [%zu] - push call handler back\n", __PRETTY_FUNCTION__, i);
            mCallHandlers.push_back (pCallHandler);
          }
        }

        void RPCClientProxy::removeCallHandlers (const uint32& n)
        {
          LOGMSG (LOW_LEVEL, "%s - n [%zu]\n", __PRETTY_FUNCTION__, n);

          for (uint32 i = 0; i < n; i++)
          {
            RPCCallHandler* pCallHandler = mCallHandlers.back ();
            mCallHandlers.pop_back ();
            LOGMSG (LOW_LEVEL, "RPCClientProxy::%s () - cancel handler [%zp]\n", __FUNCTION__, pCallHandler);
            pCallHandler->cancel ();
            LOGMSG (LOW_LEVEL, "RPCClientProxy::%s () - handler canceled [%zp]\n", __FUNCTION__, pCallHandler);
            delete pCallHandler;
          }
        }

        void RPCClientProxy::setNumCallHandlers (const uint32& n)
        {
          if (mCallHandlers.size () < n)
            addCallHandlers (n - mCallHandlers.size ());
          else if (mCallHandlers.size () > n)
            removeCallHandlers (mCallHandlers.size () - n);
        }

				void RPCClientProxy::registerRPCCall (const uint32& id, casock::rpc::protobuf::client::RPCCall* pRPCCall)
				{
					mCallHash.push (id, pRPCCall);
				}

        void RPCClientProxy::CallMethod(const google::protobuf::MethodDescriptor* method, google::protobuf::RpcController* controller, const google::protobuf::Message* request, google::protobuf::Message* response, google::protobuf::Closure* done)
        {
          LOGMSG (HIGH_LEVEL, "RPCClientProxy::%s ()\n", __FUNCTION__);

          /*!
           * TODO:
           * \todo Implement a RpcRequest factory with its own unique ID
           */
          /*
          casock::rpc::protobuf::api::RpcRequest rpcRequest;

          rpcRequest.set_id (++RPCClientProxy::mID);
          rpcRequest.set_operation (method->name ());
          rpcRequest.set_request (request->SerializeAsString ());

          sendRpcRequest (rpcRequest, new RPCCall (response, controller, done));
          */

          casock::rpc::protobuf::api::RpcRequest* pRpcRequest = mpRequestBuilder->buildRpcRequest (method, request);
          sendRpcRequest (*pRpcRequest, new RPCCall (response, controller, done));
          delete pRpcRequest;
        }
      }
    }
  }
}
