#include "vkt/ComputePipeline.h"

#include "vkt/DescriptorSetLayout.h"
#include "vkt/Device.h"

vkt::ComputePipeline::ComputePipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo )
:
    mDevice( inDevice )
{
    auto thePipelineShaderStageCreateInfo = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eFragment,
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
    mPipelineLayout = mDevice.GetVkDevice().createPipelineLayout
    (
        thePipelineLayoutCreateInfo
    );

    // Pipeline
    auto thePipelineCreateInfo = vk::ComputePipelineCreateInfo
    (
        vk::PipelineCreateFlags(),
        thePipelineShaderStageCreateInfo,
        mPipelineLayout
    );

    mPipeline = static_cast< vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >(
    mDevice.GetVkDevice().createComputePipelineUnique
        (
            vk::PipelineCache(),
            thePipelineCreateInfo
        ).value
    ).release();
}

vkt::ComputePipeline::~ComputePipeline()
{
    mDevice.GetVkDevice().destroy( mPipelineLayout );
    mDevice.GetVkDevice().destroyPipeline( mPipeline );
}

void
vkt::ComputePipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eCompute, mPipeline );
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

vkt::ComputePipelinePtr
vkt::ComputePipelineBuilder::Build()
{
    return std::make_shared< vkt::ComputePipeline >( mDevice, mPipelineCreateInfo );
}
