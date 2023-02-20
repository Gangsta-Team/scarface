CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)

project(kcp LANGUAGES C)

include(CTest)
include(GNUInstallDirs)

cmake_policy(SET CMP0054 NEW)

add_library(${PROJECT_NAME} kcp/ikcp.c)
target_include_directories(${PROJECT_NAME}
        PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/kcp/
        )