#ifndef Vkt_ComputePipeline_h
#define Vkt_ComputePipeline_h

#include "vkt/Device.h"
#include "vkt/ForwardDecl.h"
#include "vkt/Pipeline.h"

#include <vulkan/vulkan.hpp>

namespace vkt
{
    class ComputePipeline
    :
        public vkt::Pipeline
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

        private:
            std::tuple< vk::Pipeline, vk::PipelineLayout >
            CreatePipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo );

        private:
            vkt::Device const & mDevice;
    };

    class ComputePipelineBuilder
    {
        public:
            ComputePipelineBuilder( vkt::Device const & inDevice );
            ~ComputePipelineBuilder();

            ComputePipelineBuilder & SetComputeShader( vk::ShaderModule inVertexShaderModule );
            vkt::ComputePipelineBuilder & SetDescriptorSetLayouts( vkt::DescriptorSetLayoutsPtr & inDescriptorSetLayouts );
            vkt::ComputePipelineBuilder & SetPushConstants( std::vector< vk::PushConstantRange > inPushConstants );
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
