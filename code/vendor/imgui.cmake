cmake_minimum_required (VERSION 3.8)

project ("imgui")

file(GLOB ImGuiSrc
	"imgui/*.h"
	"imgui/*.cpp"
	"imgui/backends/imgui_impl_dx11.h"
	"imgui/backends/imgui_impl_dx11.cpp"
    "imgui/backends/imgui_impl_dx9.h"
	"imgui/backends/imgui_impl_dx9.cpp"
	"imgui/backends/imgui_impl_win32.h"
	"imgui/backends/imgui_impl_win32.cpp"
)

include_directories(${PROJECT_NAME} imgui/)

add_library(${PROJECT_NAME} ${ImGuiSrc})
add_library(lib::imgui ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/imgui/
)