#include <string>
#include <fstream>

#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "examples/proactor/asio/ftp/FTPSocketClient.h"
#include "examples/proactor/asio/ftp/FTPFile.h"

using casock::proactor::asio::base::AsyncProcessor;
using examples::proactor::asio::ftp::FTPSocketClient;
using examples::proactor::asio::ftp::FTPFile;

FTPFile* readfile (std::string& filename);
void sendfile (const std::string& server, const std::string& port, const FTPFile& rFile);

int main (int argc, char* argv[])
{
  LOGGER->setDebugLevel (LOW_LEVEL);
  LOGMSG (LOW_LEVEL, "%s () - start\n", __FUNCTION__);

  if (argc == 4)
  {
    std::string server    = argv[1];
    std::string port      = argv[2];
    std::string filename  = argv[3];

    LOGMSG (LOW_LEVEL, "%s () - server [%s], port [%s], filename [%s]\n", __FUNCTION__, server.c_str (), port.c_str (), filename.c_str ());
    FTPFile* pFile = readfile (filename);
    LOGMSG (LOW_LEVEL, "%s () - file size [%u]\n", __FUNCTION__, pFile->getSize ());
    sendfile (server, port, *pFile);
  }
  else
  {
    LOGMSG (LOW_LEVEL, "%s () - run: ./ftpclient <server> <port> <file>\n", __FUNCTION__);
  }
}

FTPFile* readfile (std::string& filename)
{
  FTPFile* pFile = new FTPFile (filename);

  std::ifstream is;
  is.open (filename.c_str (), std::ios::binary);

  // get size of file:
  is.seekg (0, std::ios::end);
  size_t size = is.tellg ();
  is.seekg (0, std::ios::beg);

  // allocate memory:
  char* buffer = new char [size];

  // read data as a block:
  is.read (buffer,size);

  pFile->setSize (size);
  pFile->setBuffer (buffer);

  is.close ();

  return pFile;
}

void sendfile (const std::string& server, const std::string& port, const FTPFile& rFile)
{
  AsyncProcessor::initialize ();
  AsyncProcessor* pAsyncProcessor = AsyncProcessor::getInstance ();

  try
  {
    FTPSocketClient client (*pAsyncProcessor, server, port, rFile);
    //client.sendFile (rFile);

    pAsyncProcessor->run ();
  }
  catch (...)
  {
    printf ("%s () - catch (...)\n", __FUNCTION__);
  }

  AsyncProcessor::destroy ();
}
