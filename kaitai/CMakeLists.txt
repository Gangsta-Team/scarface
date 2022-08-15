include("kaitai.cmake")

project("kaitai_generated")

file(GLOB kaitaiCode 
    "generated/*.cpp"
    "generated/*.h"
)

add_library(
    ${PROJECT_NAME}
    STATIC
    ${kaitaiCode}
)

add_library(
    lib::kaitai_generated
    ALIAS
    ${PROJECT_NAME}
)

target_link_libraries(${PROJECT_NAME} lib::kaitai)

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/generated/
)

target_compile_definitions(${PROJECT_NAME} 
    PUBLIC -DNOMINMAX
)