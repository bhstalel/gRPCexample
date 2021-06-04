
#ifndef _CHAT_H
#define _CHAT_H

#include <iostream>

#include <grpcpp/grpcpp.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;


class ChatServiceImpl final : public DeviceToolBox::Service {
    public:
        Status ToolboxCommand(ServerContext* context, const ToolboxQuery* tbReq, ServerWriter<ToolboxResult>* toolResultWriter) override;
        ToolboxServiceImpl() {}
    private:
        const   std::string     lteiface = "wwan0";
        const   std::string     ethiface = "eth0";
};

#endif // !_TOOLBOXSERVICEAPI_H
