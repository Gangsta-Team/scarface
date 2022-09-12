cmake_minimum_required(VERSION 3.0.0)
project("scarfaceModRemake")

file(GLOB ScarfaceModRemakeSrc
    "source/*.hpp"
    "source/*.cpp"
    "source/*.c"
    "source/*.h"
    "source/utils/*.*"
    "source/gameutils/*.hpp"
    "source/d3d9/*.*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/)

add_library(${PROJECT_NAME} SHARED ${ScarfaceModRemakeSrc})
target_link_libraries(${PROJECT_NAME} minhook lib::imgui lib::stackwalker lib::kaitai_generated nlohmann_json::nlohmann_json lib::filedialog lib::scarfaceLib d3d9.lib d3d11.lib)

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")