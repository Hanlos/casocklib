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
 * \file casock/rpc/protobuf/client/RPCCallTimeoutHandler.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__CASOCK_RPC_PROTOBUF_CLIENT__RPC_CALL_TIMEOUT_HANDLER_H_
#define __CASOCKLIB__CASOCK_RPC_PROTOBUF_CLIENT__RPC_CALL_TIMEOUT_HANDLER_H_

#include "casock/util/Thread.h"
#include "casock/util/Lockable.h"

namespace casock {
  namespace rpc {
    namespace protobuf {
      namespace client {
        class RPCCallHash;
        class RPCCallQueue;

        class RPCCallTimeoutHandler : public casock::util::Thread, private casock::util::Lockable
        {
          public:
            RPCCallTimeoutHandler (RPCCallHash& rCallHash, RPCCallQueue& rCallQueue)
              //: id (0), timeout (), mrCallHash (rCallHash), mrCallQueue (rCallQueue)
              : mrCallHash (rCallHash), mrCallQueue (rCallQueue)
            { }

          public:
            void run ()
            {
              /*
              lock ();

              while (true)
              {
                if (id == 0)
                  cond_wait ();
                else
                  cond_wait (timeout.tv_sec);

                if (! mrCallHash.find (id)) // or timeout already expired)
                {
                  if (mrCallHash.find (id))
                  {
                    RPCCall* pCall = mrCallHash.pop (id);
                  }
                }
              }
              */
            }

          public:
            void registerTimeout (const uint32& id, const struct timeval& timeout)
            {
            /*
              lock ();

              this->id = id;
              this->timeout = timeout;

              cond_signal ();
              unlock ();
              */
            };

          private:
            //uint32 id;
            //struct timeval timeout;
            RPCCallHash& mrCallHash;
            RPCCallQueue& mrCallQueue;
        };
      }
    }
  }
}

#endif // __CASOCKLIB__CASOCK_RPC_PROTOBUF_CLIENT__RPC_CALL_TIMEOUT_HANDLER_H_
