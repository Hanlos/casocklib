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
 * \file casock/rpc/protobuf/client/RPCClientProxy.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_PROTOBUF_CLIENT__RPC_CLIENT_PROXY_H_
#define __CASOCKLIB__CASOCK_RPC_PROTOBUF_CLIENT__RPC_CLIENT_PROXY_H_

#include <vector>

#include <google/protobuf/service.h>
#include "casock/rpc/protobuf/client/RPCCallHash.h"

namespace casock {
  namespace rpc {
    namespace protobuf {
      namespace api {
        class RpcRequest;
      }

      namespace client {
        class RPCCall;
        class RPCCallQueue;
        class RPCCallHandler;
        class RPCCallHandlerFactory;

        using casock::util::LockableHash;
        using casock::rpc::protobuf::client::RPCCall;
        using casock::rpc::protobuf::client::RPCCallQueue;
        using casock::rpc::protobuf::client::RPCCallHandler;
        using casock::rpc::protobuf::client::RPCCallHandlerFactory;

        class RPCClientProxy : public ::google::protobuf::RpcChannel
        {
          protected:
            RPCClientProxy (const RPCCallHandlerFactory& rCallHandlerFactory, const uint32& numCallHandlers = DEFAULT_NUM_CALL_HANDLERS);
            virtual ~RPCClientProxy ();

          private:
            void addCallHandlers (const uint32& n);
            void removeCallHandlers (const uint32& n);

          public:
            void setNumCallHandlers (const uint32& n);

          private:
            /*!
             * This is our way to send the message to RPC server.
             * Because we implemented our own controllers, we don't need the
             * RPC service provided by Google Protocol Buffers. We only need
             * the messages and serializations.
             *
             * The Template Method Pattern is used because the mechanisms
             * to send messages will be defined by sub-classes.
             */
            virtual void sendRpcRequest (const casock::rpc::protobuf::api::RpcRequest& request, casock::rpc::protobuf::client::RPCCall* pRPCCall) = 0;

					protected:
						void registerRPCCall (const uint32& id, casock::rpc::protobuf::client::RPCCall* pRPCCall);

          public:
            /*!
             * Overridden from ::google::protobuf::RpcChannel
             *
             * Called by google protobuf framework when some user defined
             * RPC service method is called.
             */
            void CallMethod (const ::google::protobuf::MethodDescriptor*, ::google::protobuf::RpcController*, const ::google::protobuf::Message*, ::google::protobuf::Message*, ::google::protobuf::Closure*);

          protected:
            RPCCallQueue*                   mpCallQueue;
            ::std::vector<RPCCallHandler*>  mCallHandlers;
            const RPCCallHandlerFactory&    mrCallHandlerFactory;

            /*!
             * Used to register the RPC requests, indexed by the RpcRequest ID.
             * Each RPC request should have a single ID (given by PCClientProxy::mID,
						 * that is incremented before each call).
             * The response is created by RPC server with the same ID of the request.
             * When it is received, the RPCCall is removed from the hash.
             */
            RPCCallHash	mCallHash;

          private:
            static uint32 mID;

          public:
            static uint32 DEFAULT_NUM_CALL_HANDLERS;
        };
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_PROTOBUF_CLIENT__RPC_CLIENT_PROXY_H_
