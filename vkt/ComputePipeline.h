#ifndef Vkt_ComputePipeline_h
#define Vkt_ComputePipeline_h

#include "Device.h"
#include "ForwardDecl.h"

#include <vulkan/vulkan.hpp>

namespace vkt
{
    class ComputePipeline
    {
        public:
            struct PipelineCreateInfo
            {
                vk::ShaderModule computeShaderModule;
                vkt::DescriptorSetLayoutsPtr descriptorSetLayouts;
                std::vector< vk::PushConstantRange > pushConstantRanges;
            };

        public:
            ComputePipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo );
            ~ComputePipeline();

            vk::PipelineLayout GetVkPipelineLayout();
            void Bind( vk::CommandBuffer inCommandBuffer );

        private:
            vkt::Device const & mDevice;

            vk::Pipeline mPipeline;
            vk::PipelineLayout mPipelineLayout;
    };

    class ComputePipelineBuilder
    {
        public:
            ComputePipelineBuilder( vkt::Device const & inDevice );
            ~ComputePipelineBuilder();

            ComputePipelineBuilder & SetComputeShader( vk::ShaderModule inVertexShaderModule );
            vkt::ComputePipelineBuilder & SetDescriptorSetLayouts( vkt::DescriptorSetLayoutsPtr & inDescriptorSetLayouts );
            vkt::ComputePipelinePtr Build();

        private:
            vkt::Device const & mDevice;

            ComputePipeline::PipelineCreateInfo mPipelineCreateInfo = vkt::ComputePipeline::PipelineCreateInfo {
                vk::ShaderModule(),
                nullptr,
            };
    };
}

#endif
