cmake_minimum_required(VERSION 3.0.0)
project("scarfaceRcfExplorerRemake")

file(GLOB scarfaceRcfExplorerRemakeSrc
    "source/*.hpp"
    "source/*.cpp"
    "source/*.h"
    "source/*.c"
)

add_executable(${PROJECT_NAME} ${scarfaceRcfExplorerRemakeSrc})
target_link_libraries(${PROJECT_NAME} lib::imgui lib::kaitai lib::kaitai_generated d3d11.lib)