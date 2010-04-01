#include "FTPServerSockAcceptorHandler.h"

#include <stdio.h>

#include "FTPServerSockReaderHandler.h"
#include "casock/base/Communicator.h"
#include "casock/base/FileDescriptor.h"

void FTPServerSockAcceptorHandler::createHandler (const int& fd)
{
  printf ("FTPServerSockAcceptorHandler::%s () - begin\n", __FUNCTION__);
  new FTPServerSockReaderHandler (fd);
}
