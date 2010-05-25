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
 * \file casock/rpc/protobuf/server/RPCCallHandler.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_PROTOBUF_SERVER__RPC_CALL_HANDLER_H_
#define __CASOCKLIB__CASOCK_RPC_PROTOBUF_SERVER__RPC_CALL_HANDLER_H_

#include <google/protobuf/descriptor.h>

#include "casock/util/Logger.h"
#include "casock/util/Thread.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/rpc/protobuf/server/RPCCall.h"

namespace google {
  namespace protobuf {
    class Message;
    class RpcController;
    class Service;
  }
}

namespace casock {
  namespace rpc {
    namespace protobuf {
      namespace server {
        template<typename _TpResponseHandler>
          class RPCCall;

        template<typename _TpResponseHandler>
          class RPCCallQueue;

        using casock::rpc::protobuf::api::RpcRequest;
        using casock::rpc::protobuf::api::RpcResponse;

        template<typename _TpResponseHandler>
          class RPCCallHandler : public casock::util::Thread
        {
          private:
            class RPCCallEntry
            {
              public:
                RPCCallEntry (RPCCall<_TpResponseHandler>* pCall, ::google::protobuf::Message* pResponse); //, ::google::protobuf::RpcController* pController);
                virtual ~RPCCallEntry ();

              public:
                inline RPCCall<_TpResponseHandler>* call () { return mpCall; }
                inline ::google::protobuf::Message* response () { return mpResponse; }
                //inline ::google::protobuf::RpcController* controller () { return mpController; }

              private:
                RPCCall<_TpResponseHandler>*        mpCall;
                ::google::protobuf::Message*        mpResponse;
                //::google::protobuf::RpcController*  mpController;
            };

          public:
            RPCCallHandler (RPCCallQueue<_TpResponseHandler>& rCallQueue, ::google::protobuf::Service* pService);

          public:
            static void callback (RPCCallEntry* pCallEntry);

          public:
            void run ();

          private:
            RPCCallQueue<_TpResponseHandler>& mrCallQueue;
            ::google::protobuf::Service*      mpService;
        };


        /*
         * template definitions
         */

        template<typename _TpResponseHandler>
          RPCCallHandler<_TpResponseHandler>::RPCCallEntry::RPCCallEntry (RPCCall<_TpResponseHandler>* pCall, ::google::protobuf::Message* pResponse) //, ::google::protobuf::RpcController* pController)
          : mpCall (pCall), mpResponse (pResponse)//, mpController (pController)
          { }

        template<typename _TpResponseHandler>
          RPCCallHandler<_TpResponseHandler>::RPCCallEntry::~RPCCallEntry ()
          {
            delete mpCall;
            delete mpResponse;
            //delete mpController;
          }

        template<typename _TpResponseHandler>
          RPCCallHandler<_TpResponseHandler>::RPCCallHandler (RPCCallQueue<_TpResponseHandler>& rCallQueue, ::google::protobuf::Service* pService)
          : mrCallQueue (rCallQueue), mpService (pService)
          { }

        template<typename _TpResponseHandler>
          void RPCCallHandler<_TpResponseHandler>::callback (RPCCallEntry* pCallEntry)
          {
            LOGMSG (LOW_LEVEL, "RPCCallHandler<_TpResponseHandler>::%s ()\n", __FUNCTION__);

            RPCCall<_TpResponseHandler>* pCall = pCallEntry->call ();
            ::google::protobuf::Message* pResponse = pCallEntry->response ();

            RpcResponse rpcResponse;
            rpcResponse.set_id (pCall->request ()->id ());
            rpcResponse.set_type (casock::rpc::protobuf::api::RESPONSE_TYPE_OK);
            rpcResponse.set_response (pResponse->SerializeAsString ());

            pCall->lock ();
            pCall->callback (rpcResponse);
            pCall->unlock ();

            //delete pCall;
            //delete pResponse;
            delete pCallEntry;
          }

        template<typename _TpResponseHandler>
          void RPCCallHandler<_TpResponseHandler>::run ()
          {
            while (true)
            {
              LOGMSG (LOW_LEVEL, "RPCCallHandler<_TpResponseHandler>::%s () - calling mrCallQueue.pop ()...\n", __FUNCTION__);
              RPCCall<_TpResponseHandler>* pCall = mrCallQueue.pop ();
              LOGMSG (LOW_LEVEL, "RPCCallHandler<_TpResponseHandler>::%s () - got pCall [%p]!\n", __FUNCTION__, pCall);

              const RpcRequest* const pRpcRequest = pCall->request ();

              LOGMSG (NO_DEBUG, "RPCCallhandler<_TpResponseHandler>::%s () - mpService [%p]\n", __FUNCTION__, mpService);
              LOGMSG (NO_DEBUG, "RPCCallhandler<_TpResponseHandler>::%s () - mpService->GetDescriptor () [%p]\n", __FUNCTION__, mpService->GetDescriptor ());
              LOGMSG (NO_DEBUG, "RPCCallhandler<_TpResponseHandler>::%s () - pRpcRequest [%p]\n", __FUNCTION__, pRpcRequest);

              const ::google::protobuf::MethodDescriptor* method = mpService->GetDescriptor ()->FindMethodByName (pRpcRequest->operation ());

              if (method != NULL)
              {
                LOGMSG (NO_DEBUG, "RPCCallHandler<_TpResponseHandler>::%s () - OK\n", __FUNCTION__);

                ::google::protobuf::Message* request = mpService->GetRequestPrototype (method).New ();
                request->ParseFromString (pRpcRequest->request ());

                /*!
                 * I didn't see anybody using a controller in the server side yet.
                 * So, just for now, let's set it as NULL.
                 */
                ::google::protobuf::RpcController* controller = NULL;
                ::google::protobuf::Message* response = mpService->GetResponsePrototype (method).New ();

                /*!
                 * The RPCCallEntry will be deleted on RPCCallHandler<_TpResponseHandler>::callback (RPCCallEntry*).
                 */
                ::google::protobuf::Closure* closure = ::google::protobuf::NewCallback (RPCCallHandler<_TpResponseHandler>::callback, new RPCCallEntry (pCall, response)); //, controller));

                mpService->CallMethod (method, controller, request, response, closure);

                delete request;
              }
              else
                LOGMSG (NO_DEBUG, "RPCCallHandler<_TpResponseHandler>::%s () - no method called [%s]\n", __FUNCTION__, pRpcRequest->operation ().c_str ());
            }
          }
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_PROTOBUF_SERVER__RPC_CALL_HANDLER_H_
