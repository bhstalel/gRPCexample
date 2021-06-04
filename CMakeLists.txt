cmake_minimum_required (VERSION 3.5)

set(GRPCEXAMPLE_VERSUIN "0.0.1")

# Set a default build type if none was specified
if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
  message("Setting build type to 'RelWithDebInfo' as none was specified.")
  set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
  # Set the possible values of build type for cmake-gui
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
    "MinSizeRel" "RelWithDebInfo")
endif()

project(gRPCexample VERSION ${GRPCEXAMPLE_VERSUIN})

# set the C++ standard to C++ 11
set(CMAKE_CXX_STANDARD 11)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

include(CompileProtos)
set(protobuf_MODULE_COMPATIBLE TRUE)
find_package(Protobuf REQUIRED)

set(_PROTOBUF_LIBPROTOBUF protobuf::libprotobuf)
if(CMAKE_CROSSCOMPILING)
  find_program(_PROTOBUF_PROTOC protoc)
  find_program(_gRPC_CPP_PLUGIN grpc_cpp_plugin)
else()
  set(_PROTOBUF_PROTOC $<TARGET_FILE:protobuf::protoc>)
endif()

# Looks for gRPCConfig.cmake file installed by gRPC's cmake installation.
find_package(GRPC REQUIRED)

set(_GRPC_GRPCPP_REFLECTION gRPC::grpc++_reflection)
if(CMAKE_CROSSCOMPILING)
  find_program(_GRPC_CPP_PLUGIN_EXECUTABLE grpc_cpp_plugin)
else()
  set(_GRPC_CPP_PLUGIN_EXECUTABLE $<TARGET_FILE:gRPC::grpc_cpp_plugin>)
endif()

#Add sub-directories
add_subdirectory(protos)
#add_subdirectory(systemd)
#add_subdirectory(services)