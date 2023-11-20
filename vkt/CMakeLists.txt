set(
    HEADER_FILES
        Buffer.h
        ComputePipeline.h
        Device.h
        DescriptorSetLayout.h
        ForwardDecl.h
        FrameBuffer.h
        GraphicsPipeline.h
        Image.h
        Instance.h
        Mesh.h
        Pipeline.h
        PhysicalDevice.h
        RenderPass.h
        Shader.h
        Swapchain.h
        Vma.h
)

set(
    SOURCE_FILES
        Source/Buffer.cpp
        Source/ComputePipeline.cpp
        Source/Device.cpp
        Source/DescriptorSetLayout.cpp
        Source/FrameBuffer.cpp
        Source/GraphicsPipeline.cpp
        Source/Image.cpp
        Source/Instance.cpp
        Source/Mesh.cpp
        Source/Pipeline.cpp
        Source/PhysicalDevice.cpp
        Source/RenderPass.cpp
        Source/Shader.cpp
        Source/Swapchain.cpp
)

add_library(
    Vkt SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET Vkt PROPERTY CXX_STANDARD 17 )
include_directories( ${BURST_ROOT_DIR} )

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Vkt PUBLIC spdlog::spdlog )

# Vulkan
find_package( Vulkan REQUIRED )
target_link_libraries( Vkt PUBLIC Vulkan::Vulkan )