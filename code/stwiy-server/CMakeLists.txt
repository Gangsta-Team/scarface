cmake_minimum_required(VERSION 3.0.0)
project("stwiyServer")

set(CMAKE_CXX_STANDARD  17)

set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)

file(GLOB StwiyServerSrc
    "source/*.*"
)

include_directories(${PROJECT_NAME} ${CMAKE_CURRENT_SOURCE_DIR}/source/)
include_directories(${PROJECT_NAME} ../stwiy-net/src/)
include_directories(${PROJECT_NAME} ../vendor/enet/)
include_directories(${PROJECT_NAME} ../vendor/enet/includes/)

add_executable(${PROJECT_NAME} ${StwiyServerSrc})
target_link_libraries(${PROJECT_NAME} nlohmann_json::nlohmann_json sf_net)
set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".exe")