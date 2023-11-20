#ifndef Vkt_GraphicsPipeline_h
#define Vkt_GraphicsPipeline_h

#include "vkt/Device.h"
#include "vkt/DescriptorSetLayout.h"
#include "vkt/Pipeline.h"

namespace vkt
{
    class GraphicsPipeline
    :
        public vkt::Pipeline
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

            friend class GraphicsPipelineBuilder;

        private:
            std::tuple< vk::Pipeline, vk::PipelineLayout > CreatePipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo );
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
            GraphicsPipelineBuilder & SetVertexInputBindingDescriptions( std::vector< vk::VertexInputBindingDescription > inVertexInputBindingDescriptions );
            GraphicsPipelineBuilder & SetVertexInputAttributeDescriptions( std::vector< vk::VertexInputAttributeDescription > inVertexInputAttributeDescriptions );
            GraphicsPipelineBuilder & SetPushConstants( std::vector< vk::PushConstantRange > inPushConstants );

            GraphicsPipelinePtr Build();

        private:
            vkt::Device const & mDevice;
            GraphicsPipeline::PipelineCreateInfo mPipelineCreateInfo;
    };
}

#endif
