#include "vkt/GraphicsPipeline.h"

vkt::GraphicsPipeline::GraphicsPipeline( vkt::Device const & inDevice, PipelineCreateInfo const & inCreateInfo )
:
    mDevice( inDevice )
{
    std::array< vk::PipelineShaderStageCreateInfo, 2 > thePipelineShaderStageCreateInfos;
    thePipelineShaderStageCreateInfos[ 0 ] = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eVertex,
        inCreateInfo.vertexShaderModule,
        "main"
    );

    thePipelineShaderStageCreateInfos[ 1 ] = vk::PipelineShaderStageCreateInfo
    (
        vk::PipelineShaderStageCreateFlags(),
        vk::ShaderStageFlagBits::eFragment,
        inCreateInfo.fragmentShaderModule,
        "main"
    );

    // Vertex binding
    auto const theVertexInputStateCreateInfo = vk::PipelineVertexInputStateCreateInfo
    (
        vk::PipelineVertexInputStateCreateFlags(),
        inCreateInfo.vertexInputBindingDescriptions.size(),
        inCreateInfo.vertexInputBindingDescriptions.data(),
        inCreateInfo.vertexInputAttributeDescriptions.size(),
        inCreateInfo.vertexInputAttributeDescriptions.data()
    );

    // Input assembly
    auto const theInputAssemblyStateCreateInfo = vk::PipelineInputAssemblyStateCreateInfo
    (
        vk::PipelineInputAssemblyStateCreateFlags(),
        inCreateInfo.topology,
        VK_FALSE
    );

    // Viewport
    auto const theViewport = vk::Viewport();
    auto const theScissor = vk::Rect2D();
    auto const theViewportStateCreateInfo = vk::PipelineViewportStateCreateInfo
    (
        vk::PipelineViewportStateCreateFlags(),
        1,
        &theViewport,
        1,
        &theScissor
    );

    // Rasterization
    auto const theRasterizationStateCreateInfo = vk::PipelineRasterizationStateCreateInfo
    (
        vk::PipelineRasterizationStateCreateFlags(),
        false,
        false,
        vk::PolygonMode::eFill,
        vk::CullModeFlagBits::eNone,
        vk::FrontFace::eCounterClockwise,
        false,
        0.0f,
        0.0f,
        0.0f,
        1.0f
    );

    // Multi sample
    auto const theMultiSampleStateCreateInfo = vk::PipelineMultisampleStateCreateInfo
    (
        vk::PipelineMultisampleStateCreateFlags(),
        vk::SampleCountFlagBits::e1,
        VK_FALSE
    );

    // Depth stencil
    auto const theDepthStencilStateCreateInfo = vk::PipelineDepthStencilStateCreateInfo
    (
        vk::PipelineDepthStencilStateCreateFlags(),
        true,
        true,
        vk::CompareOp::eLess,
        false,
        false,
        vk::StencilOp::eKeep,
        vk::StencilOp::eKeep,
        0.0f,
        1.0f
    );

    // Color blend
    std::vector< vk::PipelineColorBlendAttachmentState > theColorBlendAttachmentStates =
    {
        vk::PipelineColorBlendAttachmentState
        (
            true,
            vk::BlendFactor::eSrcAlpha,
            vk::BlendFactor::eOneMinusSrcAlpha,
            vk::BlendOp::eAdd,
            vk::BlendFactor::eOne,
            vk::BlendFactor::eOne,
            vk::BlendOp::eAdd,
            vk::ColorComponentFlagBits::eR | vk::ColorComponentFlagBits::eG | vk::ColorComponentFlagBits::eB | vk::ColorComponentFlagBits::eA
        )
    };
    auto const theColorBlendStateCreateInfo = vk::PipelineColorBlendStateCreateInfo
    (
        vk::PipelineColorBlendStateCreateFlags(),
        false,
        vk::LogicOp::eCopy,
        theColorBlendAttachmentStates.size(),
        theColorBlendAttachmentStates.data(),
        { 0.0f, 0.0f, 0.0f, 0.0f }
    );

    // Dynamic states
    std::vector< vk::DynamicState > theDynamicStates =
    {
        vk::DynamicState( vk::DynamicState::eViewport ),
        vk::DynamicState( vk::DynamicState::eScissor )
    };
    auto const theDynamicStateCreateInfo = vk::PipelineDynamicStateCreateInfo
    (
        vk::PipelineDynamicStateCreateFlags(),
        theDynamicStates.size(),
        theDynamicStates.data()
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
    auto thePipelineCreateInfo = vk::GraphicsPipelineCreateInfo
    (
        vk::PipelineCreateFlags(),
        thePipelineShaderStageCreateInfos.size(),
        thePipelineShaderStageCreateInfos.data(),
        &theVertexInputStateCreateInfo,
        &theInputAssemblyStateCreateInfo,
        nullptr,
        &theViewportStateCreateInfo,
        &theRasterizationStateCreateInfo,
        &theMultiSampleStateCreateInfo,
        &theDepthStencilStateCreateInfo,
        &theColorBlendStateCreateInfo,
        &theDynamicStateCreateInfo,
        mPipelineLayout,
        inCreateInfo.renderPass,
        0,
        nullptr,
        -1
    );

    mPipeline = static_cast< vk::UniqueHandle< vk::Pipeline, VULKAN_HPP_DEFAULT_DISPATCHER_TYPE > >(
        mDevice.GetVkDevice().createGraphicsPipelineUnique
        (
            vk::PipelineCache(),
            thePipelineCreateInfo
        ).value
    ).release();
}

vkt::GraphicsPipeline::~GraphicsPipeline()
{
    mDevice.GetVkDevice().destroy( mPipelineLayout );
    mDevice.GetVkDevice().destroyPipeline( mPipeline );
}

void
vkt::GraphicsPipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( vk::PipelineBindPoint::eGraphics, mPipeline );
}

vk::PipelineLayout
vkt::GraphicsPipeline::GetVkPipelineLayout() const
{
    return mPipelineLayout;
}

// ============================================ GraphicPipelineBuilder ===============================================

vkt::GraphicsPipelineBuilder::GraphicsPipelineBuilder( const vkt::Device & inDevice )
:
    mDevice( inDevice ),
    mPipelineCreateInfo( vkt::GraphicsPipeline::PipelineCreateInfo {
        vk::ShaderModule(),
        vk::ShaderModule(),
        std::vector< vk::VertexInputBindingDescription >(),
        std::vector< vk::VertexInputAttributeDescription >(),
        vk::PrimitiveTopology::eTriangleList,
        vk::RenderPass(),
        nullptr,
        std::vector< vk::PushConstantRange >()
    } )
{
    mPipelineCreateInfo.topology = vk::PrimitiveTopology::eTriangleList;
}

vkt::GraphicsPipelineBuilder::~GraphicsPipelineBuilder() = default;



vkt::GraphicsPipelineBuilder &
vkt::GraphicsPipelineBuilder::SetRenderPass( vk::RenderPass inRenderPass )
{
    mPipelineCreateInfo.renderPass = inRenderPass;
    return *this;
}

vkt::GraphicsPipelineBuilder &
vkt::GraphicsPipelineBuilder::SetVertexShader( vk::ShaderModule inVertexShaderModule )
{
    mPipelineCreateInfo.vertexShaderModule = inVertexShaderModule;
    return *this;
}

vkt::GraphicsPipelineBuilder &
vkt::GraphicsPipelineBuilder::SetFragmentShader( vk::ShaderModule inFragmentShaderModule )
{
    mPipelineCreateInfo.fragmentShaderModule = inFragmentShaderModule;
    return *this;
}

vkt::GraphicsPipelineBuilder &
vkt::GraphicsPipelineBuilder::SetDescriptorSetLayouts( vkt::DescriptorSetLayoutsPtr & inDescriptorSetLayouts )
{
    mPipelineCreateInfo.descriptorSetLayouts = inDescriptorSetLayouts;
    return *this;
}

vkt::GraphicsPipelinePtr
vkt::GraphicsPipelineBuilder::Build()
{
    return std::make_shared< vkt::GraphicsPipeline >( mDevice, mPipelineCreateInfo );
}
