#include <stdlib.h>

#include "casock/util/Logger.h"
#include "casock/base/Dispatcher.h"
#include "casock/base/CASException.h"
#include "casock/server/ServerSocket.h"
#include "casock/server/CASServerException.h"
#include "FTPServerSockAcceptorHandler.h"

void waitforfiles (const unsigned short& port);

int main (int argc, char* argv[])
{
  LOGGER->setDebugLevel (LOW_LEVEL);
  LOGMSG (LOW_LEVEL, "main () - start\n");

  if (argc == 2)
  {
    unsigned short port = atoi (argv[1]);
    waitforfiles (port);
  }
  else
  {
    LOGMSG (LOW_LEVEL, "main () - run: ./ftpserver <port>\n");
  }
}


void waitforfiles (const unsigned short& port)
{
  casock::base::Dispatcher::initialize ();

  try
  {
    casock::server::ServerSocket server (port);

    server.listen ();

    FTPServerSockAcceptorHandler handler (&server);
    casock::base::Dispatcher::getInstance ()->waitForever ();

    server.close ();
    handler.unregister ();
  }
  catch (casock::server::CASServerException& e)
  {
    LOGMSG (NO_DEBUG, "main () - CASServerException [%s]\n", e.what ());
  }
  catch (casock::base::CASException& e)
  {
    LOGMSG (NO_DEBUG, "main () - CASException [%s]\n", e.what ());
  }

  casock::base::Dispatcher::destroy ();
}
