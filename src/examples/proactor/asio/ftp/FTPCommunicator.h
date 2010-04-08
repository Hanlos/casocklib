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
 * \file examples/proactor/asio/ftp/FTPSocketClient.h
 * \brief [brief description]
 * \author Leandro Costa
 * \date 2010
 *
 * $LastChangedDate$
 * $LastChangedBy$
 * $Revision$
 */

#ifndef __CASOCKLIB__EXAMPLES_PROACTOR_ASIO_FTP__FTP_COMMUNICATOR_H_
#define __CASOCKLIB__EXAMPLES_PROACTOR_ASIO_FTP__FTP_COMMUNICATOR_H_

#include "casock/proactor/asio/base/Communicator.h"
#include "examples/proactor/asio/ftp/FTPFile.h"

namespace casock {
  namespace proactor {
    namespace asio {
      namespace base {
        class SocketChannel;
      }
    }
  }
}

namespace examples {
  namespace proactor {
    namespace asio {
      namespace ftp {
        class FTPCommunicator : public casock::proactor::asio::base::Communicator
        {
          public:
            FTPCommunicator (casock::proactor::asio::base::SocketChannel* const pChannel)
              : casock::proactor::asio::base::Communicator (pChannel)
            { }

          private:
            void onSentSize (const ::asio::error_code& error, const FTPFile& rFile, ::boost::function<void(const ::asio::error_code&)> handler)
            {
              if (! error)
              {
                write (rFile.getBuffer (), rFile.getSize (), ::boost::bind (&FTPCommunicator::onSentBuffer, this, ::asio::placeholders::error, handler));
              }
              else
              {
                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
              }
            }

            void onSentBuffer (const ::asio::error_code& error, ::boost::function<void(const ::asio::error_code&)> handler)
            {
              if (! error)
              {
                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - NO ERROR!\n", __FUNCTION__);
              }
              else
              {
                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - error [%s]\n", __FUNCTION__, error.message ().c_str ());
              }

              handler (error);
            }
 
          public:
            void sendFile (const FTPFile& rFile, ::boost::function<void(const ::asio::error_code&)> handler)
            {
              write (rFile.getSize (), ::boost::bind (&FTPCommunicator::onSentSize, this, ::asio::placeholders::error, rFile, handler));
            }

            /*
            FTPFile* getFile (const unsigned int& counter)
            {
              ssize_t s = 0;

              if (! size)
              {
                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - ! size\n", __FUNCTION__);
                s = Communicator::read (reinterpret_cast<char *>(&size), sizeof (size_t));
                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - s [%zd]\n", __FUNCTION__, s);

                if (! s)
                  throw (casock::base::CASClosedConnectionException ());
                else if (s < 0)
                  throw (casock::base::CASException ("Unfinished message!"));

                size = ntohl (size);

                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - receiving message with %Zu bytes\n", __FUNCTION__, size);
              }

              do
              {
                s = Communicator::read (buffer, size - buffer.str ().length ());
                LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - s [%zd]\n", __FUNCTION__, s);
              } while (s > 0 && buffer.str ().length () < size);

              LOGMSG (LOW_LEVEL, "FTPCommunicator::%s () - received %zu/%zu bytes\n", __FUNCTION__, buffer.str ().length (), size);

              if (buffer.str ().length () < (size_t) size)
                throw (casock::base::CASException ("Unfinished message!"));

              char* ftpbuff = new char [size];
              strncpy (ftpbuff, buffer.str ().c_str (), size);

              stringstream ss;
              ss << "file_";
              ss << counter;
              ss << ".ftp";

              string filename = ss.str ();

              FTPFile* pFile = new FTPFile (filename);
              pFile->setSize (size);
              pFile->setBuffer (ftpbuff);

              size = 0;

              return pFile;
            }

            const size_t& getSize () { return size; }
            const size_t getBuffSize () { return buffer.str ().length (); }

          private:
            size_t size;
            stringstream buffer;
            */
        };
      }
    }
  }
}

#endif // __CASOCKLIB__EXAMPLES_PROACTOR_ASIO_FTP__FTP_COMMUNICATOR_H_
