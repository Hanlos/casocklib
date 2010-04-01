#ifndef __CASOCKLIB__EXAMPLES_FTP_FTP_CLIENT_SOCK_READER_HANDLER_H_
#define __CASOCKLIB__EXAMPLES_FTP_FTP_CLIENT_SOCK_READER_HANDLER_H_

#include "casock/base/Handler.h"
#include "FTPCommunicator.h"

namespace casock {
  namespace base {
    class FileDescriptor;
  }
}

class FTPClientSockReaderHandler : public casock::base::Handler
{
  public:
    FTPClientSockReaderHandler (const casock::base::FileDescriptor* const pFileDescriptor) : Handler (pFileDescriptor), mCommunicator (mpFileDescriptor) { }

//  private:
//    void destroy () { delete this; }

  public:
    void handle ();
    FTPCommunicator& communicator () { return mCommunicator; }

  private:
    FTPCommunicator mCommunicator;
};


#endif // __CASOCKLIB__EXAMPLES_FTP_FTP_CLIENT_SOCK_READER_HANDLER_H_
