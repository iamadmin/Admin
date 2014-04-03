ADD_EXECUTABLE(agentd agentd.cpp)
TARGET_LINK_LIBRARIES(agentd util model)
ADD_EXECUTABLE(masterd masterd.cpp)
TARGET_LINK_LIBRARIES(masterd util model control)
ADD_EXECUTABLE(client client.cpp)
TARGET_LINK_LIBRARIES(client util model)

INSTALL(TARGETS agentd masterd client
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        )

