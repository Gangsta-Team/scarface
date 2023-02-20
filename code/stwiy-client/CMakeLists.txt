cmake_minimum_required(VERSION 3.0.0)
project("stwiyClient")

file(GLOB StwiyClientSrc
    "source/*.hpp"
    "source/*.cpp"
    "source/*.c"
    "source/*.h"
    "source/net/*.*"
    "source/utils/*.*"
    "source/gameutils/*.hpp"
    "source/d3d9/*.*"
    "source/natives/*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/)

add_library(${PROJECT_NAME} SHARED ${StwiyClientSrc})
target_link_libraries(${PROJECT_NAME} minhook lib::imgui nlohmann_json::nlohmann_json lib::stwiyLib lib::event_system d3d9.lib d3d11.lib kcp concurrentqueue)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")