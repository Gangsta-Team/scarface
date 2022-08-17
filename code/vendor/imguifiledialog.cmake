cmake_minimum_required (VERSION 3.8)

project ("filedialog")

file(GLOB FileDialogSrc
	"ImGuiFileDialog/*.cpp"
    "ImGuiFileDialog/*.h"
)

add_library(${PROJECT_NAME} ${FileDialogSrc})
add_library(lib::filedialog ALIAS ${PROJECT_NAME})

target_include_directories(${PROJECT_NAME}
	PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog/
)

target_link_libraries(${PROJECT_NAME} lib::imgui)