
#ifndef _CHAT_H
#define _CHAT_H

#include <iostream>

#include <grpcpp/grpcpp.h>
#include <gRPCexample.grpc.pb.h>

using grpc::Server;
using grpc::ServerBuilder;
using grpc::ServerContext;
using grpc::Status;
using grpc::ServerWriter;

using namespace gRPCexample;

class ChatServiceImpl final : public gRPCexample::Chat::Service {
    public:
        Status SaySomething(ServerContext* context, const gRPCexample::RequestMessage* request, gRPCexample::ReplyMessage* response) override;
        ChatServiceImpl() {}
};

#endif
