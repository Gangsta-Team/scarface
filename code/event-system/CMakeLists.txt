cmake_minimum_required(VERSION 3.0.0)
project("eventSystem")

file(GLOB EventSystemSrc
    "source/*.*"
    "include/*.*"
)

include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_library(${PROJECT_NAME} STATIC ${EventSystemSrc})
add_library(lib::event_system ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)