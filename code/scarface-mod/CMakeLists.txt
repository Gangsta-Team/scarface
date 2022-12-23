cmake_minimum_required(VERSION 3.0.0)
project("scarfaceModRemake")

option(SCARFACE_MOD_NATIVES_ENABLE_DUMPER "Enable the Tool to dump natives!" ON)

file(GLOB ScarfaceModRemakeSrc
    "source/*.hpp"
    "source/*.cpp"
    "source/*.c"
    "source/*.h"
    "source/utils/*.*"
    "source/gameutils/*.hpp"
    "source/d3d9/*.*"
    "source/tools/nativedumper/*"
    "source/natives/*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/)

add_library(${PROJECT_NAME} SHARED ${ScarfaceModRemakeSrc})
target_link_libraries(${PROJECT_NAME} minhook lib::imgui lib::stackwalker lib::kaitai_generated nlohmann_json::nlohmann_json lib::filedialog lib::scarfaceLib lib::event_system d3d9.lib d3d11.lib)

if(SCARFACE_MOD_NATIVES_ENABLE_DUMPER)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _ENABLE_NATIVES_DUMPER=1)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".asi")