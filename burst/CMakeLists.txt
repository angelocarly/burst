set(
    HEADER_FILES
        AssetLoader.h
        Display.h
        Engine.h
        Gui.h
        GuiPresenter.h
        Image.h
        Presenter.h
        ShaderEditor.h
        Utils.h
        VulkanConfig.h
        Window.h
)

set(
    SOURCE_FILES
        Source/AssetLoader.cpp
        Source/Display.cpp
        Source/Engine.cpp
        Source/Gui.cpp
        Source/GuiPresenter.cpp
        Source/Image.cpp
        Source/ShaderEditor.cpp
        Source/Utils.cpp
        Source/VulkanConfig.cpp
        Source/Window.cpp
)

add_library(
    Burst SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET Burst PROPERTY CXX_STANDARD 20 )
include_directories( ${BURST_ROOT_DIR} )

# Project libraries
target_link_libraries(
    Burst PUBLIC
        Vkt
        Imgui
        ImGuiFileDialog
)

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Burst PUBLIC spdlog::spdlog )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( Burst PUBLIC glfw )

# stb
set( STB_DIR "${BURST_ROOT_DIR}/lib/stb" )
include_directories( ${STB_DIR} )

# zep
set( ZEP_DIR "${BURST_ROOT_DIR}/lib/zep" )
target_include_directories(
    Burst
        PRIVATE
        ${ZEP_DIR}/include
)
add_definitions(-DZEP_SINGLE_HEADER=1)
# Set this to get the default file system
add_definitions(-DZEP_FEATURE_CPP_FILE_SYSTEM)

target_link_libraries(Burst PRIVATE Zep::Zep)
