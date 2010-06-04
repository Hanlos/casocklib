#include "casock/util/Logger.h"
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/protobuf/server/RPCCallHandlerFactoryImpl.h"
#include "casock/rpc/asio/protobuf/server/RPCServerProxy.h"

#include "examples/rpc/protobuf/api/rpc_hello.pb.h"

casock::rpc::asio::protobuf::server::RPCServerProxy* proxy;

class HelloServiceImpl : public  HelloService
{
  public:
    void HelloCall(::google::protobuf::RpcController* controller,
        const ::HelloRequest* request,
        ::HelloResponse* response,
        ::google::protobuf::Closure* done)
    {
      LOGMSG (LOW_LEVEL, "HelloServiceImpl::%s () - request->message () [%s]\n", __FUNCTION__, request->message ().c_str ());

      response->set_id (request->id ());
      response->set_message ("I'm the server. You sent me: " + request->message ());
      done->Run ();

      if (request->message () == "shutdown")
      {
        sleep (1);
        proxy->stop ();
      }
    }
};

int main ()
{
  LOGGER->setDebugLevel (MAX_LEVEL);
  LOGMSG (LOW_LEVEL, "%s () - start\n", __FUNCTION__);

  casock::proactor::asio::base::AsyncProcessor::initialize ();

  HelloServiceImpl service;
  casock::rpc::protobuf::server::RPCCallHandlerFactoryImpl callHandlerFactory (&service);

  try
  {
    casock::proactor::asio::base::AsyncProcessor* pAsyncProcessor = casock::proactor::asio::base::AsyncProcessor::getInstance ();
    proxy = new casock::rpc::asio::protobuf::server::RPCServerProxy (callHandlerFactory, *pAsyncProcessor, 2000);
    proxy->setNumCallHandlers (1);
    proxy->start ();

    pAsyncProcessor->run ();
  }
  catch (...)
  {
    LOGMSG (NO_DEBUG, "%s () - catch (...)\n", __FUNCTION__);
  }

  delete proxy;
  casock::proactor::asio::base::AsyncProcessor::destroy ();
  LOGGER->finalize ();
}
