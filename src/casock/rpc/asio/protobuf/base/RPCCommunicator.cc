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
 * \file casock/rpc/asio/protobuf/base/RPCCommunicator.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/rpc/asio/protobuf/base/RPCCommunicator.h"

//#include <arpa/inet.h>
#include <boost/bind.hpp>

#include <google/protobuf/message.h>

#include "casock/util/Logger.h"
#include "casock/base/CASClosedConnectionException.h"
#include "casock/rpc/protobuf/api/rpc.pb.h"
#include "casock/rpc/base/CASRPCException.h"
#include "casock/rpc/base/CASRPCUnfinishedMessageException.h"

namespace casock {
  namespace rpc {
    namespace asio {
      namespace protobuf {
        namespace base {
          RPCCommunicator::RPCCommunicator (SocketChannel* const pChannel) : casock::proactor::asio::base::Communicator (pChannel)
          {

          }

          void RPCCommunicator::onSentSize (const ::asio::error_code& error, const std::stringstream* buffer, ::boost::function<void(const ::asio::error_code&)> handler)
          //void RPCCommunicator::onSentSize (const ::asio::error_code& error, const std::stringstream* buffer, ::boost::function<void()> handler)
          {
            LOGMSG (LOW_LEVEL, "RPCCommunicator::%s () - error [%d]\n", __FUNCTION__, error.value ());

            if (! error)
            {
              LOGMSG (LOW_LEVEL, "RPCCommunicator::%s () - sending message with %d bytes\n", __FUNCTION__, buffer->str ().size ());
              write (buffer->str ().c_str (), buffer->str ().length (), ::boost::bind (&RPCCommunicator::onSentBuffer, this, ::asio::placeholders::error, buffer, handler));
            }
            else
            {
              LOGMSG (LOW_LEVEL, "RPCCommunicator::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
            }
          }

          void RPCCommunicator::onSentBuffer (const ::asio::error_code& error, const std::stringstream* buffer, ::boost::function<void(const ::asio::error_code&)> handler)
          //void RPCCommunicator::onSentBuffer (const ::asio::error_code& error, const std::stringstream* buffer, ::boost::function<void()> handler)
          {
            LOGMSG (LOW_LEVEL, "RPCCommunicator::%s ()\n", __FUNCTION__);
            delete buffer;
            handler (error);
            //handler ();
          }

          void RPCCommunicator::onReadSize (const ::asio::error_code& error, ::boost::function<void(const ::asio::error_code&, google::protobuf::Message*)> handler)
          {
            LOGMSG (LOW_LEVEL, "RPCCommunicator::%s ()\n", __FUNCTION__);

            if (! error)
            {
              LOGMSG (LOW_LEVEL, "RPCCommunicator::%s () - mSize [%zd]\n", __FUNCTION__, mSize);
              char* buffer = new char [mSize];
              read (buffer, mSize, ::boost::bind (&RPCCommunicator::onReadBuffer, this, ::asio::placeholders::error, buffer, handler));
            }
            else
            {
              LOGMSG (LOW_LEVEL, "RPCCommunicator::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
            }
          }

          void RPCCommunicator::onReadBuffer (const ::asio::error_code& error, char* buffer, ::boost::function<void(const ::asio::error_code&, google::protobuf::Message*)> handler)
          {
            LOGMSG (LOW_LEVEL, "RPCCommunicator::%s ()\n", __FUNCTION__);

            google::protobuf::Message* pResponse = NULL;

            if (! error)
            {
              pResponse = createRecvMessage ();
              std::stringstream ss (string (buffer, mSize));
              pResponse->ParseFromIstream (&ss);
              delete[] buffer;
            }
            else
            {
              LOGMSG (LOW_LEVEL, "RPCCommunicator::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
            }

            handler (error, pResponse);
          }

          void RPCCommunicator::sendMessage (const ::google::protobuf::Message* const message, ::boost::function<void(const ::asio::error_code&)> handler)
          //void RPCCommunicator::sendMessage (const ::google::protobuf::Message* const message, ::boost::function<void()> handler)
          {
            LOGMSG (NO_DEBUG, "RPCCommunicator::%s () - sending size [%zd]\n", __FUNCTION__, message->ByteSize ());
            std::stringstream* buffer = new std::stringstream ();
            message->SerializeToOstream (buffer);
            write (message->ByteSize (), ::boost::bind (&RPCCommunicator::onSentSize, this, ::asio::placeholders::error, buffer, handler));
          }

          void RPCCommunicator::recvMessage (::boost::function<void(const ::asio::error_code&, ::google::protobuf::Message*)> handler)
          {
            LOGMSG (NO_DEBUG, "RPCCommunicator::%s () - receiving size...\n", __FUNCTION__);
            read (mSize, ::boost::bind (&RPCCommunicator::onReadSize, this, ::asio::placeholders::error, handler));
          }
        }
      }
    }
  }
}
