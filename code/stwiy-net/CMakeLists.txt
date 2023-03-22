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
    src/common.hpp
    src/network_client.cpp
    src/network_client.hpp
    src/network_server.cpp
    src/network_server.hpp
)

add_library(${SF_NET_LIB_MAIN} ${SF_NET_SOURCES})
set_target_properties(${SF_NET_LIB_MAIN} PROPERTIES PREFIX "")
target_include_directories(${SF_NET_LIB_MAIN} PUBLIC src/)

# enet
target_link_libraries(${SF_NET_LIB_MAIN} PUBLIC Ws2_32.lib winmm.lib enet)
target_include_directories(${SF_NET_LIB_MAIN} PUBLIC ../vendor/enet/)
target_include_directories(${SF_NET_LIB_MAIN} PUBLIC ../vendor/enet/include/)

IF (SF_NET_DEBUG)
    message([SF_NET] Debug)
    target_compile_definitions(${SF_NET_LIB_MAIN} PUBLIC SF_NET_DEBUG)
ELSE ()
    message([SF_NET] Release)
ENDIF ()

# Example App
add_subdirectory(demo)