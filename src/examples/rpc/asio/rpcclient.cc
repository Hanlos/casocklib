#include "casock/util/Logger.h"
#include "casock/proactor/asio/base/AsyncProcessor.h"
//#include "casock/rpc/protobuf/client/RPCCallController.h"
//#include "casock/rpc/sigio/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/protobuf/client/RPCResponseHandler.h"
#include "examples/rpc/protobuf/api/rpc_hello.pb.h"

using casock::proactor::asio::base::AsyncProcessor;

void Done ();
void Done (HelloResponse* pResponse);

class HelloHandler : public casock::rpc::protobuf::client::RPCResponseHandler
{
  public:
    HelloHandler (casock::rpc::protobuf::client::RPCCallController* pController, HelloResponse* pResponse) : casock::rpc::protobuf::client::RPCResponseHandler (pController), mpResponse (pResponse) { }

  public:
    void callback ()
    {
      LOGMSG (NO_DEBUG, "HelloHandler::%s () - message [%s]\n", __FUNCTION__, mpResponse->message ().c_str ());
    }

  private:
    HelloResponse* mpResponse;
};

int main ()
{
  LOGGER->setDebugLevel (LOW_LEVEL);
  LOGMSG (LOW_LEVEL, "%s () - start\n", __FUNCTION__);

  AsyncProcessor::initialize ();
  AsyncProcessor* pAsyncProcessor = AsyncProcessor::getInstance ();

  try
  {
    /*
    casock::rpc::sigio::protobuf::client::RPCClientProxy proxy (*pDispatcher, "localhost", 2000);
    casock::rpc::protobuf::client::RPCCallController controller;

    HelloService* service = new HelloService::Stub (&proxy);

    HelloRequest request;
    HelloResponse response;

    request.set_id (1);
    request.set_message ("Hello!");

    HelloHandler handler (&controller, &response);
    //service->HelloCall (&controller, &request, &response, google::protobuf::NewCallback (&Done, &response));
    service->HelloCall (&controller, &request, &response, handler.closure ());

    //Dispatcher::getInstance ()->wait (1);
    pDispatcher->waitForever ();

    delete service;
    */
  }
  catch (...)
  {
    LOGMSG (NO_DEBUG, "%s () - catch (...)\n", __FUNCTION__);
  }

  AsyncProcessor::destroy ();
}

void Done ()
{
  LOGMSG (NO_DEBUG, "Done ()\n");
}

void Done (HelloResponse* pResponse)
{
  LOGMSG (NO_DEBUG, "Done () - message [%s]\n", pResponse->message ().c_str ());
}
