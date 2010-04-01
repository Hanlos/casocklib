#ifndef __CASOCKLIB__EXAMPLES_FTP_FTP_SERVER_SOCK_ACCEPTOR_HANDLER_H_
#define __CASOCKLIB__EXAMPLES_FTP_FTP_SERVER_SOCK_ACCEPTOR_HANDLER_H_

#include "casock/server/SockAcceptorHandler.h"

namespace casock {
  namespace base {
    class FileDescriptor;
  }
}

class FTPServerSockAcceptorHandler : public casock::server::SockAcceptorHandler
{
  public:
    FTPServerSockAcceptorHandler (const casock::base::FileDescriptor* const pFileDescriptor) : casock::server::SockAcceptorHandler (pFileDescriptor) { }

  private:
//    void createHandler (const FileDescriptor* const pFileDescriptor);
    void createHandler (const int& fd);
};

#endif // __CASOCKLIB__EXAMPLES_FTP_FTP_SERVER_SOCK_ACCEPTOR_HANDLER_H_
