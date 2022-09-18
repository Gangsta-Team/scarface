cmake_minimum_required(VERSION 3.0.0)
project("scarfaceLib")

file(GLOB ScarfaceLibSrc
    "*.hpp"
    "*.cpp"
    "*.h"
    "*.c"
    "torque3d/*.*"
    "pure3d/*.*"
    "scripting/*.*"
    "gameobject/*.*"
    "gameobject/spawnobject/*.*"
    "gameobject/render/*.*"
)


add_library(${PROJECT_NAME} STATIC ${ScarfaceLibSrc})
add_library(lib::scarfaceLib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/)