project(chat)

add_library(${PROJECT_NAME} "src/chat.cpp")

add_library(lib::chat ALIAS ${PROJECT_NAME})

target_include_directories( ${PROJECT_NAME}
    PUBLIC ${PROJECT_SOURCE_DIR}/include
)

target_link_libraries(${PROJECT_NAME}
    lib::common
    grpc::toolbox
    gRPC::grpc++
    config++
    pthread
    tins
)
