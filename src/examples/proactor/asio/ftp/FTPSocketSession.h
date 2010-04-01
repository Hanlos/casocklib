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
 * \file examples/proactor/asio/FTPSocketSession.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__EXAMPLES_PROACTOR_ASIO_FTP_FTP_SOCKET_SESSION_H_
#define __CASOCKLIB__EXAMPLES_PROACTOR_ASIO_FTP_FTP_SOCKET_SESSION_H_

#include <sstream>
#include <iomanip>
#include <boost/bind.hpp>

#include "casock/util/Logger.h"
#include "casock/proactor/asio/server/SocketSession.h"
#include "examples/proactor/asio/ftp/FTPFile.h"

namespace examples {
  namespace proactor {
    namespace asio {
      namespace ftp {
        class FTPSocketSession : public casock::proactor::asio::server::SocketSession
        {
          public:
            FTPSocketSession (casock::proactor::asio::base::AsyncProcessor& rAsyncProcessor)
              : casock::proactor::asio::server::SocketSession (rAsyncProcessor)
            {
              LOGMSG (LOW_LEVEL, "FTPSocketSession::FTPSocketSession ()\n");
              mFileSize = 0;
              mpFileBuffer = NULL;
            }

          private:
            void onConnect ()
            {
              LOGMSG (LOW_LEVEL, "FTPSocketSession::%s ()\n", __FUNCTION__);
              read (mFileSize, ::boost::bind (&FTPSocketSession::onReadSize, this, ::asio::placeholders::error));
            }

            void onReadSize (const ::asio::error_code& error)
            {
              if (! error)
              {
                LOGMSG (LOW_LEVEL, "FTPSocketSession::%s () - mFileSize [%zu]\n", __FUNCTION__, mFileSize);

                if (mFileSize)
                {
                  mpFileBuffer = new char [mFileSize];
                  read (mpFileBuffer, mFileSize, ::boost::bind (&FTPSocketSession::onReadBuffer, this, ::asio::placeholders::error));
                }
                else
                {
                  read (mFileSize, ::boost::bind (&FTPSocketSession::onReadSize, this, ::asio::placeholders::error));
                }
              }
              else
              {
                LOGMSG (LOW_LEVEL, "FTPSocketSession::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
                delete this;
              }
            }

            void onReadBuffer (const ::asio::error_code& error)
            {
              if (! error)
              {
                LOGMSG (LOW_LEVEL, "FTPSocketSession::%s () - file size [%zu]\n", __FUNCTION__, mFileSize);

                std::stringstream s;
                s << "file_" << std::setfill ('0') << std::setw (2) << mCounter++;

                FTPFile file (s.str ());
                file.setSize (mFileSize);
                file.setBuffer (mpFileBuffer);
                file.save ();

                delete mpFileBuffer;
                mpFileBuffer = NULL;
                mFileSize = 0;

                read (mFileSize, ::boost::bind (&FTPSocketSession::onReadSize, this, ::asio::placeholders::error));
              }
              else
              {
                LOGMSG (LOW_LEVEL, "FTPSocketSession::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
                delete mpFileBuffer;
                delete this;
              }
            }

          private:
            size_t mFileSize;
            char* mpFileBuffer;
            static unsigned long mCounter;
        };

        unsigned long FTPSocketSession::mCounter = 0;
      }
    }
  }
}

#endif // __CASOCKLIB__EXAMPLES_PROACTOR_ASIO_FTP_FTP_SOCKET_SESSION_H_
