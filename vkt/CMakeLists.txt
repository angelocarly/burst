set(
    HEADER_FILES
        Vma.h
        Device.h
        ForwardDecl.h
        Image.h
        Instance.h
        PhysicalDevice.h
        Swapchain.h
)

set(
    SOURCE_FILES
        Source/Device.cpp
        Source/Image.cpp
        Source/Instance.cpp
        Source/PhysicalDevice.cpp
        Source/Swapchain.cpp
)

add_library(
    Vkt SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET Vkt PROPERTY CXX_STANDARD 17 )
include_directories( ${PROJECT_ROOT_DIR} )

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Vkt PUBLIC spdlog::spdlog )

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( Vkt PUBLIC Vulkan::Vulkan )