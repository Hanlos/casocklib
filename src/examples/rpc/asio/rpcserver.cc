#include "casock/util/Logger.h"
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/asio/protobuf/server/RPCServerProxy.h"

#include "examples/rpc/protobuf/api/rpc_hello.pb.h"

using casock::proactor::asio::base::AsyncProcessor;

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
    }
};

int main ()
{
  LOGGER->setDebugLevel (LOW_LEVEL);
  LOGMSG (LOW_LEVEL, "%s () - start\n", __FUNCTION__);

  AsyncProcessor::initialize ();
  AsyncProcessor* pAsyncProcessor = AsyncProcessor::getInstance ();

  try
  {
    casock::rpc::asio::protobuf::server::RPCServerProxy proxy (*pAsyncProcessor, 2000, new HelloServiceImpl ());
    proxy.start ();

    pAsyncProcessor->run ();
  }
  catch (...)
  {
    LOGMSG (NO_DEBUG, "%s () - catch (...)\n", __FUNCTION__);
  }

  AsyncProcessor::destroy ();
}
