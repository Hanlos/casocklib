#ifndef __CASOCKLIB__EXAMPLES_FTP_FTP_SERVER_SOCK_READER_HANDLER_H_
#define __CASOCKLIB__EXAMPLES_FTP_FTP_SERVER_SOCK_READER_HANDLER_H_

#include <stdlib.h>

#include "casock/base/Handler.h"
#include "FTPCommunicator.h"

class FTPServerSockReaderHandler : public casock::base::Handler
{
  public:
    FTPServerSockReaderHandler (const int& fd);

  private:
    void destroy () { delete this; }

  public:
    void handle ();

  private:
    FTPCommunicator mCommunicator;
    size_t received;

    static unsigned int counter;
};


#endif // __CASOCKLIB__EXAMPLES_FTP_FTP_SERVER_SOCK_READER_HANDLER_H_
