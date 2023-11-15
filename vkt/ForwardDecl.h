#ifndef Vkt_ForwardDecl_h
#define Vkt_ForwardDecl_h

#include <memory>

namespace vkt
{
    class ComputePipeline;
    using ComputePipelinePtr = std::shared_ptr< ComputePipeline >;

    class Device;

    class DescriptorSetLayouts;
    using DescriptorSetLayoutsPtr = std::shared_ptr< DescriptorSetLayouts >;

    class FrameBuffer;
    using FrameBufferPtr = std::shared_ptr< FrameBuffer >;

    class Image;
    using ImagePtr = std::shared_ptr< Image >;

    class GraphicsPipeline;
    using GraphicsPipelinePtr = std::shared_ptr< GraphicsPipeline >;

    class RenderPass;
    using RenderPassPtr = std::shared_ptr< RenderPass >;
}

#endif
