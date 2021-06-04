project(gRPCexample)

google_cloud_cpp_grpcpp_library(gRPCexample_grpc "gRPCexample.proto" PROTO_PATH_DIRECTORIES "./;${CMAKE_SYSROOT}/usr/include")

# This is a static library that you can link to your gRPC services
add_library(grpc::gRPCexample ALIAS gRPCexample_grpc)