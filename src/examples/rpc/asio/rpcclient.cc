#include "casock/util/Logger.h"
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/protobuf/client/RPCCallController.h"
#include "casock/rpc/asio/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/protobuf/client/RPCResponseHandler.h"
#include "casock/base/CASClosedConnectionException.h"
#include "examples/rpc/protobuf/api/rpc_hello.pb.h"

void Done ();
void Done (casock::rpc::protobuf::client::RPCCallController* pController, HelloResponse* pResponse);

casock::rpc::asio::protobuf::client::RPCClientProxy* proxy;
HelloService* service;
HelloRequest request;

class HelloHandler : public casock::rpc::protobuf::client::RPCResponseHandler
{
  public:
    HelloHandler (casock::rpc::protobuf::client::RPCCallController* pController, HelloResponse* pResponse)
      //: casock::rpc::protobuf::client::RPCResponseHandler (pController), mpResponse (pResponse)
      : mpResponse (pResponse)
    { }

  public:
    void callback ()
    {
      LOGMSG (NO_DEBUG, "HelloHandler::%s () - message [%s]\n", __FUNCTION__, mpResponse->message ().c_str ());

      request.set_id (2);
      request.set_message ("shutdown");
      HelloResponse* response = new HelloResponse ();
      casock::rpc::protobuf::client::RPCCallController* controller = new casock::rpc::protobuf::client::RPCCallController ();

      service->HelloCall (controller, &request, response, ::google::protobuf::NewCallback (&Done, controller, response));
    }

  private:
    HelloResponse* mpResponse;
};

int main ()
{
  LOGGER->setDebugLevel (MAX_LEVEL);
  LOGMSG (LOW_LEVEL, "%s () - start\n", __FUNCTION__);

  casock::proactor::asio::base::AsyncProcessor::initialize ();

  try
  {
    casock::proactor::asio::base::AsyncProcessor* pAsyncProcessor = casock::proactor::asio::base::AsyncProcessor::getInstance ();
    proxy = new casock::rpc::asio::protobuf::client::RPCClientProxy (*pAsyncProcessor, "localhost", "2000");

    service = new HelloService::Stub (proxy);

    HelloResponse* response = new HelloResponse ();
    casock::rpc::protobuf::client::RPCCallController* controller = new casock::rpc::protobuf::client::RPCCallController ();

    request.set_id (1);
    request.set_message ("Hello!");

    HelloHandler handler (controller, response);
    LOGMSG (NO_DEBUG, "%s () - service->HelloCall (...)\n", __FUNCTION__);

    try
    {
      service->HelloCall (controller, &request, response, handler.closure ());
    }
    catch (casock::base::CASClosedConnectionException& e)
    {
      LOGMSG (NO_DEBUG, "%s () - casock::base::CASClosedConnectionException [%s]\n", __FUNCTION__, e.what ());
    }

    pAsyncProcessor->run ();

    delete service;
  }
  catch (...)
  {
    LOGMSG (NO_DEBUG, "%s () - catch (...)\n", __FUNCTION__);
  }

  delete proxy;
  casock::proactor::asio::base::AsyncProcessor::destroy ();
}

void Done ()
{
  LOGMSG (NO_DEBUG, "Done ()\n");
}

void Done (casock::rpc::protobuf::client::RPCCallController* pController, HelloResponse* pResponse)
{
  LOGMSG (NO_DEBUG, "Done () - message [%s]\n", pResponse->message ().c_str ());
  delete pController;
  delete pResponse;
}
