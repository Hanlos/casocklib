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
 * \file casock/proactor/asio/client/SocketClient.cc
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#include "casock/proactor/asio/client/SocketClient.h"

#include <boost/bind.hpp>

#include "casock/util/Logger.h"
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/base/CASClosedConnectionException.h"

namespace casock {
  namespace proactor {
    namespace asio {
      namespace client {
        SocketClient::SocketClient (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor, const std::string& host, const string& port)
          : casock::proactor::asio::base::SocketChannel (rAsyncProcessor), m_resolver (rAsyncProcessor.service ()), m_query (host, port)
        {
          LOGMSG (LOW_LEVEL, "SocketClient::SocketClient () - host [%s], port [%s]\n", host.c_str (), port.c_str ());

          /*
          //::asio::ip::tcp::resolver resolver (rAsyncProcessor.service ());
          ::asio::ip::tcp::resolver::query query (host, port);

          m_resolver.async_resolve (query,
              ::boost::bind (&SocketClient::handle_resolve, this, ::asio::placeholders::error, ::asio::placeholders::iterator));
              */
        }

        void SocketClient::handle_resolve (const ::asio::error_code& error, ::asio::ip::tcp::resolver::iterator it_endpoint)
        {
          LOGMSG (LOW_LEVEL, "SocketClient::%s ()\n", __FUNCTION__);

          if (! error)
          {
            ::asio::ip::tcp::endpoint endpoint = *it_endpoint;
            m_socket.async_connect (endpoint,
                ::boost::bind (&SocketClient::handle_connect, this, ::asio::placeholders::error, ++it_endpoint));
          }
          else
          {
            LOGMSG (LOW_LEVEL, "SocketClient::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
          }
        }

        void SocketClient::handle_connect (const ::asio::error_code& error, ::asio::ip::tcp::resolver::iterator it_endpoint)
        {
          LOGMSG (LOW_LEVEL, "SocketClient::%s ()\n", __FUNCTION__);

          if (! error)
          {
            LOGMSG (LOW_LEVEL, "SocketClient::%s () - CONNECTED!\n", __FUNCTION__);
            onConnect ();
          }
          else if (it_endpoint != ::asio::ip::tcp::resolver::iterator ())
          {
            LOGMSG (LOW_LEVEL, "SocketClient::%s () - no connected, trying again...\n", __FUNCTION__);

            close ();
            ::asio::ip::tcp::endpoint endpoint = *it_endpoint;
            m_socket.async_connect (endpoint,
                ::boost::bind (&SocketClient::handle_connect, this, ::asio::placeholders::error, ++it_endpoint));
          }
          else
            LOGMSG (LOW_LEVEL, "SocketClient::%s () - NO CONNECTED!\n", __FUNCTION__);
        }

        void SocketClient::syncConnect ()
        {
          LOGMSG (LOW_LEVEL, "SocketClient::%s ()\n", __FUNCTION__);

          ::asio::ip::tcp::resolver::iterator it = m_resolver.resolve (m_query);
          ::asio::ip::tcp::resolver::iterator itEnd;

          //::boost::system::error_code error = ::boost::asio::error::host_not_found;

          //while (error && it != itEnd)
          while (! m_socket.is_open () && it != itEnd)
          {
            m_socket.close ();
            //m_socket.connect (*it, error);
            m_socket.connect (*it);
          }

          if (! m_socket.is_open ())
          {
            throw (casock::base::CASClosedConnectionException ("impossible to connect"));
          }
          else
            LOGMSG (LOW_LEVEL, "SocketClient::%s () - socket connected!\n");

          //if (error)
          //  throw (::boost::system::system_error (error));
        }

        void SocketClient::asyncConnect ()
        {
          m_resolver.async_resolve (m_query,
              ::boost::bind (&SocketClient::handle_resolve, this, ::asio::placeholders::error, ::asio::placeholders::iterator));
        }
      }
    }
  }
}
