An asynchronous communication library for C++ under construction. It contains:

  * A SIGIO based communication framework with asynchronous reading;
  * A Proactor based communication framework implemented with Boost.Asio;
  * A Google Protocol Buffers based RPC framework implemented with both, SIGIO and Boost.Asio Proactor communication frameworks;
  * Examples of how each subset of the library should be used.

The user can choose which subset of the library it needs, for example, it can use only the SIGIO based communication framework, or the RPC framework implemented with Google Protocol Buffers and Boost.Asio.

# A Boost.Asio / Google Protocol Buffers RPC Service #

With **casocklib** we have an RPC service framework using a [Proactor](http://www.cs.wustl.edu/~schmidt/PDF/proactor.pdf) based communication service implemented with [Boost.Asio](http://www.boost.org/doc/libs/1_42_0/doc/html/boost_asio.html) and [Google Protocol Buffers](http://code.google.com/intl/pt-BR/apis/protocolbuffers/docs/overview.html).

To implement an RPC service using casocklib the user should execute the following steps:

  * Define an RPC service using Google Protocol Buffers based on its documentation http://code.google.com/intl/pt-BR/apis/protocolbuffers/docs/proto.html#services;

For the server:

  * Implement a class with the operations defined by the RPC service;
  * Use the **RPCServerProxy**, provided by casocklib, passing an instance of the RPC service. The RPCServerProxy will be responsible to call the operations provided by the RPC service according to the requests received from the client.

For the client:

  * According to Google Protocol Buffers documentation, create a service using the **RPCClientProxy**, provided by casocklib, as an [RpcChannel](http://code.google.com/intl/pt-BR/apis/protocolbuffers/docs/reference/cpp/google.protobuf.service.html#RpcChannel);
  * Call the RPC service operations using a controller provided by casocklib and a closure for callback.

## Quick Example ##

The RPC service defined:

--- file rpc\_hello.proto ---
```
message HelloRequest {
  required uint32 id = 1;
  required string message = 2;
}

message HelloResponse {
  required uint32 id = 1;
  required string message = 2;
}

service HelloService {
  rpc HelloCall (HelloRequest) returns (HelloResponse);
}
```

The server side:

--- file rpcserver.cc ---
```
#include <iostream>
#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/asio/protobuf/server/RPCServerProxy.h"

#include "examples/rpc/protobuf/api/rpc_hello.pb.h"

using namespace std;
using casock::proactor::asio::base::AsyncProcessor;
using casock::rpc::asio::protobuf::server::RPCServerProxy;

class HelloServiceImpl : public  HelloService
{
  public:
    void HelloCall(::google::protobuf::RpcController* controller,
        const ::HelloRequest* request,
        ::HelloResponse* response,
        ::google::protobuf::Closure* done)
    {
      response->set_id (request->id ());
      response->set_message ("I'm the server. You sent me: " + request->message ());
      done->Run ();
    }
};

int main ()
{
  AsyncProcessor::initialize ();
  RPCServerProxy* proxy = NULL;
  HelloServiceImpl service;
  AsyncProcessor* pAsyncProcessor = AsyncProcessor::getInstance ();

  try
  {
    proxy = new RPCServerProxy (*pAsyncProcessor, 2000, &service);
    proxy->start ();

    pAsyncProcessor->run ();
  }
  catch (exception& e)
  {
    cout << "main () - catch (exception&): " << e.what () << cout::endl;
  }

  if (proxy)
    delete proxy;

  AsyncProcessor::destroy ();
}
```

The client side:

--- file rpcclient.cc ---
```
#include <iostream>

#include "casock/proactor/asio/base/AsyncProcessor.h"
#include "casock/rpc/protobuf/client/RPCCallController.h"
#include "casock/rpc/asio/protobuf/client/RPCClientProxy.h"
#include "casock/rpc/protobuf/client/RPCResponseHandler.h"
#include "examples/rpc/protobuf/api/rpc_hello.pb.h"

using namespace std;
using casock::proactor::asio::base::AsyncProcessor;
using casock::rpc::protobuf::client::RPCCallController;
using casock::rpc::protobuf::client::RPCResponseHandler;
using casock::rpc::asio::protobuf::client::RPCClientProxy;

class HelloHandler : public RPCResponseHandler
{
  public:
    HelloHandler (RPCCallController* pController, HelloResponse* pResponse)
      : RPCResponseHandler (pController), mpResponse (pResponse)
    { }

  public:
    void callback ()
    {
      cout << "response message: " << mpResponse->message () << endl;
    }

  private:
    HelloResponse* mpResponse;
};

int main ()
{
  AsyncProcessor::initialize ();
  RPCClientProxy* proxy = NULL;
  HelloService* service = NULL;
  AsyncProcessor* pAsyncProcessor = AsyncProcessor::getInstance ();

  try
  {
    proxy = new RPCClientProxy (*pAsyncProcessor, "localhost", "2000");
    service = new HelloService::Stub (proxy);

    HelloResponse* response = new HelloResponse ();
    RPCCallController* controller = new RPCCallController ();

    HelloRequest request;
    request.set_id (1);
    request.set_message ("Hello!");

    HelloHandler handler (controller, response);

    service->HelloCall (controller, &request, response, handler.closure ());

    pAsyncProcessor->run ();
  }
  catch (exception& e)
  {
    cout << "catch (exception& e): " << e.what () << endl;
  }

  if (service)
    delete service;

  if (proxy)
    delete proxy;

  AsyncProcessor::destroy ();
}
```