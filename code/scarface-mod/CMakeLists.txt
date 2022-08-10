cmake_minimum_required(VERSION 3.0.0)
project(scarfaceMod)

file(GLOB ScarfaceModSource
    "source/*.hpp"
    "source/*.cpp"
    "source/*.h"
    "source/*.c"
    "source/Torque3D/*.*"
    "source/Torque3D/*.*"
)

add_library(${PROJECT_NAME} SHARED ${ScarfaceModSource})
target_link_libraries(${PROJECT_NAME} minhook d3d9.lib d3d11.lib)

set_target_properties(scarfaceMod PROPERTIES SUFFIX ".asi")