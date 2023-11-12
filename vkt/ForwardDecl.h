#ifndef Vkt_ForwardDecl_h
#define Vkt_ForwardDecl_h

#include <memory>

namespace vkt
{
    class Device;

    class DescriptorSetLayouts;
    using DescriptorSetLayoutsPtr = std::shared_ptr< DescriptorSetLayouts >;

    class FrameBuffer;
    using FrameBufferPtr = std::shared_ptr< FrameBuffer >;

    class Image;

    class GraphicsPipeline;
    using GraphicsPipelinePtr = std::shared_ptr< GraphicsPipeline >;

    class RenderPass;
    using RenderPassPtr = std::shared_ptr< RenderPass >;
}

#endif
