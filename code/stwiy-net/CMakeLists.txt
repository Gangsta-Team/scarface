cmake_minimum_required(VERSION 3.9)
project(sf_net)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(SF_NET_LIB_MAIN sf_net)

message([SF_NET] Project: ${PROJECT_NAME})
message([SF_NET] BuildType: ${CMAKE_BUILD_TYPE})
message([SF_NET] CMAKE_CXX_COMPILER_ID: ${CMAKE_CXX_COMPILER_ID})
message([SF_NET] CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME})

# config - start
set(SF_NET_DEBUG TRUE)
# config - end

set(SF_NET_SOURCES
        src/net_buffer.cpp
        src/net_buffer.hpp
        src/net_client.cpp
        src/net_client.hpp
        src/net_event.hpp
        src/net_event_serializer.cpp
        src/net_event_serializer.hpp
        src/net_server.cpp
        src/net_server.hpp
        src/net_socket.hpp
        src/net_socket_win.cpp
        )

add_library(${SF_NET_LIB_MAIN} ${SF_NET_SOURCES})
set_target_properties(${SF_NET_LIB_MAIN} PROPERTIES PREFIX "")
target_include_directories(${SF_NET_LIB_MAIN} PUBLIC src/)

# concurrentqueue
add_subdirectory(third_party/concurrentqueue)
target_link_libraries(${SF_NET_LIB_MAIN} PUBLIC concurrentqueue)

# kcp
add_subdirectory(third_party/kcp)
target_link_libraries(${SF_NET_LIB_MAIN} PUBLIC kcp)
target_include_directories(${SF_NET_LIB_MAIN} PUBLIC third_party/kcp/)

IF (SF_NET_DEBUG)
    message([SF_NET] Debug)
    target_compile_definitions(${SF_NET_LIB_MAIN} PUBLIC SF_NET_DEBUG)
ELSE ()
    message([SF_NET] Release)
ENDIF ()

# Example App
add_subdirectory(demo)