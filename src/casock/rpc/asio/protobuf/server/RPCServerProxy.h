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
 * \file casock/rpc/asio/protobuf/server/RPCServerProxy.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_PRORPC_SERVER_RPC_SERVER_PROXY_H_
#define __CASOCKLIB__CASOCK_PRORPC_SERVER_RPC_SERVER_PROXY_H_

namespace google {
  namespace protobuf {
    class Service;
  }
}

namespace casock {
  namespace proactor {
    namespace asio {
      namespace base {
        class AsyncProcessor;
      }
    }
  }

  namespace rpc {
    namespace protobuf {
      namespace server {
        class RPCCallResponseHandler;

        template<typename _TpResponseHandler>
          class RPCCallQueue;

        template<typename _TpResponseHandler>
          class RPCCallHandler;
      }
    }
  }

  namespace prorpc {
    namespace server {
      class RPCSocketServer;

      using casock::rpc::protobuf::server::RPCCallQueue;
      using casock::rpc::protobuf::server::RPCCallHandler;
      using casock::rpc::protobuf::server::RPCCallResponseHandler;

      class RPCServerProxy
      {
        public:
          RPCServerProxy (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const unsigned int& port, google::protobuf::Service* pService);
          virtual ~RPCServerProxy ();

        public:
          void start ();
          void stop ();

        private:
          RPCSocketServer*                        mpSocketServer;
          RPCCallQueue<RPCCallResponseHandler>*   mpCallQueue;
          RPCCallHandler<RPCCallResponseHandler>* mpCallHandler;

          google::protobuf::Service*              mpService;

          bool m_running;
      };
    }
  }
}

#endif // __CASOCKLIB__CASOCK_PRORPC_SERVER_RPC_SERVER_PROXY_H_
