project(chat_service)

add_executable(${PROJECT_NAME} chat_service.cpp)

# Create a static library for the service
target_link_libraries(${PROJECT_NAME}
    lib::chat
)

# install binary
install (TARGETS ${PROJECT_NAME}
    DESTINATION /bin)