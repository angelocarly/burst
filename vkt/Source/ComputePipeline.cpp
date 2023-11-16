#include "vkt/ComputePipeline.h"

#include "vkt/DescriptorSetLayout.h"
#include "vkt/Device.h"

vkt::ComputePipeline::ComputePipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo )
:
    vkt::Pipeline( inDevice, vk::PipelineBindPoint::eCompute, CreatePipeline( inDevice, inCreateInfo ) ),
    mDevice( inDevice )
{
}

std::tuple< vk::Pipeline, vk::PipelineLayout >
vkt::ComputePipeline::CreatePipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo )
{
    auto thePipelineShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eCompute,
        inCreateInfo.computeShaderModule,
        "main"
    );

    // Pipeline Layout
    auto thePipelineLayoutCreateInfo = vk::PipelineLayoutCreateInfo
    (
        vk::PipelineLayoutCreateFlags(),
        inCreateInfo.descriptorSetLayouts ? inCreateInfo.descriptorSetLayouts->GetVkDescriptorSetLayouts().size() : 0,
        inCreateInfo.descriptorSetLayouts ? inCreateInfo.descriptorSetLayouts->GetVkDescriptorSetLayouts().data() : nullptr,
        inCreateInfo.pushConstantRanges.size(),
        inCreateInfo.pushConstantRanges.data()
    );
    auto pipelineLayout = inDevice.GetVkDevice().createPipelineLayout
    (
        thePipelineLayoutCreateInfo
    );

    // Pipeline
    auto thePipelineCreateInfo = vk::ComputePipelineCreateInfo
    (
        vk::PipelineCreateFlags(),
        thePipelineShaderStageCreateInfo,
        pipelineLayout
    );

    auto pipeline = static_cast< vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >(
    inDevice.GetVkDevice().createComputePipelineUnique
    (
        vk::PipelineCache(),
        thePipelineCreateInfo
    ).value
    ).release();

    return { pipeline, pipelineLayout };
}

vkt::ComputePipeline::~ComputePipeline()
{
}

// ============================================ GraphicPipelineBuilder ===============================================

vkt::ComputePipelineBuilder::ComputePipelineBuilder( const vkt::Device & inDevice )
:
    mDevice( inDevice )
{
}

vkt::ComputePipelineBuilder::~ComputePipelineBuilder() = default;

vkt::ComputePipelineBuilder &
vkt::ComputePipelineBuilder::SetComputeShader( vk::ShaderModule inVertexShaderModule )
{
    mPipelineCreateInfo.computeShaderModule = inVertexShaderModule;
    return *this;
}

vkt::ComputePipelineBuilder &
vkt::ComputePipelineBuilder::SetDescriptorSetLayouts( vkt::DescriptorSetLayoutsPtr & inDescriptorSetLayouts )
{
    mPipelineCreateInfo.descriptorSetLayouts = inDescriptorSetLayouts;
    return *this;
}

vkt::ComputePipelineBuilder &
vkt::ComputePipelineBuilder::SetPushConstants( std::vector< vk::PushConstantRange > inPushConstants )
{
    mPipelineCreateInfo.pushConstantRanges = std::move( inPushConstants );
    return *this;
}

vkt::ComputePipelinePtr
vkt::ComputePipelineBuilder::Build()
{
    return std::make_shared< vkt::ComputePipeline >( mDevice, mPipelineCreateInfo );
}
