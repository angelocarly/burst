set(
    HEADER_FILES
        Display.h
        Engine.h
        GuiPresenter.h
        Presenter.h
        Utils.h
        VulkanConfig.h
        Window.h
)

set(
    SOURCE_FILES
        Source/Display.cpp
        Source/Engine.cpp
        Source/GuiPresenter.cpp
        Source/Utils.cpp
        Source/VulkanConfig.cpp
        Source/Window.cpp
)

add_library(
    Burst SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET Burst PROPERTY CXX_STANDARD 17 )
include_directories( ${BURST_ROOT_DIR} )

# Project libraries
target_link_libraries(
    Burst PUBLIC
        Vkt
        Imgui
)

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Burst PUBLIC spdlog::spdlog )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( Burst PUBLIC glfw )