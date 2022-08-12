cmake_minimum_required (VERSION 3.8)

project ("stackwalker")

file(GLOB StackwalkerSrc 
    "StackWalker/Main/StackWalker/StackWalker.h"
    "StackWalker/Main/StackWalker/StackWalker.cpp"
)

add_library(${PROJECT_NAME} STATIC ${StackwalkerSrc})
add_library(lib::stackwalker ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/StackWalker/Main/
)