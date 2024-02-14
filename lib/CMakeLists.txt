## IMGUI
set( IMGUI_DIR ${CMAKE_CURRENT_SOURCE_DIR}/imgui )
add_library( Imgui STATIC )
set_property( TARGET Imgui PROPERTY CXX_STANDARD 20 )
set_target_properties( Imgui PROPERTIES POSITION_INDEPENDENT_CODE ON )

target_sources(
    Imgui
        PRIVATE
        ${IMGUI_DIR}/imgui_demo.cpp
        ${IMGUI_DIR}/imgui_draw.cpp
        ${IMGUI_DIR}/imgui_tables.cpp
        ${IMGUI_DIR}/imgui_widgets.cpp
        ${IMGUI_DIR}/imgui.cpp

        PRIVATE
        ${IMGUI_DIR}/backends/imgui_impl_glfw.cpp
        ${IMGUI_DIR}/backends/imgui_impl_vulkan.cpp
)

target_include_directories(
    Imgui
        PUBLIC ${IMGUI_DIR}
        PUBLIC ${IMGUI_DIR}/backends
)

find_package( glfw3 3.3 REQUIRED )
target_link_libraries( Imgui PUBLIC glfw )

find_package( Vulkan REQUIRED )
target_link_libraries( Imgui PUBLIC Vulkan::Vulkan )

# ImGuiFileDialog
set( IMGUIFILEDIALOG_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ImGuiFileDialog )
add_library( ImGuiFileDialog STATIC )
set_property( TARGET ImGuiFileDialog PROPERTY CXX_STANDARD 20 )
target_link_libraries( ImGuiFileDialog PUBLIC Imgui )
target_sources(
    ImGuiFileDialog
        PRIVATE
        ${IMGUIFILEDIALOG_DIR}/ImGuiFileDialog.cpp
)
target_include_directories(
    ImGuiFileDialog
        PUBLIC ${IMGUIFILEDIALOG_DIR}
)

# stb
include_directories( ${STB_DIR} )

# zep
set( ZEP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/zep )
add_subdirectory( ${ZEP_DIR} )
