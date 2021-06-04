project(gRPCexample)

configure_file(gRPCexample.service.in gRPCexample.service)
install (FILES ${PROJECT_BINARY_DIR}/gRPCexample.service DESTINATION lib/systemd/system)