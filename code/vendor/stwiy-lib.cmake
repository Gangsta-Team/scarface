project("stwiyLib")

file(GLOB stwiyLibSrc
    "stwiy-lib/*.hpp"
    "stwiy-lib/*.cpp"
    "stwiy-lib/*.h"
    "stwiy-lib/*.c"
    "stwiy-lib/torque3d/*.*"
    "stwiy-lib/pure3d/*.*"
    "stwiy-lib/scripting/*.*"
    "stwiy-lib/gameobject/*.*"
    "stwiy-lib/engine/*.*"
    "stwiy-lib/engine/object/*.*"
    "stwiy-lib/gameobject/spawnobject/*.*"
    "stwiy-lib/gameobject/render/*.*"
    "stwiy-lib/gameobject/camera/*.*"
    "stwiy-lib/gameobject/character/*.*"
)

include_directories(${PROJECT_NAME} stwiy-lib/)

add_library(${PROJECT_NAME} STATIC ${stwiyLibSrc})
add_library(lib::stwiyLib ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/stwiy-lib/
)

target_compile_definitions(${PROJECT_NAME} 
    PRIVATE -DKS_STR_ENCODING_NONE
)

target_compile_definitions(${PROJECT_NAME} 
    PUBLIC -DNOMINMAX
)