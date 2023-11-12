#ifndef Vkt_GraphicsPipeline_h
#define Vkt_GraphicsPipeline_h

#include "vkt/Device.h"
#include "vkt/DescriptorSetLayout.h"

namespace vkt
{
    class GraphicsPipeline
    {
        private:
            struct PipelineCreateInfo
            {
                vk::ShaderModule vertexShaderModule;
                vk::ShaderModule fragmentShaderModule;
                std::vector< vk::VertexInputBindingDescription > vertexInputBindingDescriptions;
                std::vector< vk::VertexInputAttributeDescription > vertexInputAttributeDescriptions;
                vk::PrimitiveTopology topology;
                vk::RenderPass renderPass;
                vkt::DescriptorSetLayoutsPtr descriptorSetLayouts;
                std::vector< vk::PushConstantRange > pushConstantRanges;
            };

        public:
            GraphicsPipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo );
            ~GraphicsPipeline();

            void Bind( vk::CommandBuffer inCommandBuffer );

            friend class GraphicsPipelineBuilder;

        private:
            vkt::Device const & mDevice;
            vk::PipelineLayout mPipelineLayout;
            vk::Pipeline mPipeline;
    };

    class GraphicsPipelineBuilder
    {
        public:
            GraphicsPipelineBuilder( vkt::Device const & inDevice );
            ~GraphicsPipelineBuilder();

            GraphicsPipelineBuilder & SetRenderPass( vk::RenderPass inRenderPass );
            GraphicsPipelineBuilder & SetVertexShader( vk::ShaderModule inVertexShaderModule );
            GraphicsPipelineBuilder & SetFragmentShader( vk::ShaderModule inFragmentShaderModule );
            GraphicsPipelineBuilder & SetDescriptorSetLayouts( vkt::DescriptorSetLayoutsPtr & inDescriptorSetLayouts );

            GraphicsPipelinePtr Build();

        private:
            vkt::Device const & mDevice;
            GraphicsPipeline::PipelineCreateInfo mPipelineCreateInfo;
    };
}

#endif
