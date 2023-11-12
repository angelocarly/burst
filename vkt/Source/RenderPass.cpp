#include "vkt/RenderPass.h"

vkt::RenderPass::RenderPass( const vkt::Device & inDevice, vk::Format inImageFormat )
:
    mDevice( inDevice )
{
    auto theColorAttachmentReference = vk::AttachmentReference
    (
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        & theColorAttachmentReference,
        nullptr,
        nullptr,
        0,
        nullptr
    );

    // The depth attachment is first accessed in the early fragment stage.
    std::array< vk::SubpassDependency, 1 > theSubPassDependencies;
    theSubPassDependencies[ 0 ].setSrcSubpass( VK_SUBPASS_EXTERNAL );
    theSubPassDependencies[ 0 ].setDstSubpass( 0 );
    theSubPassDependencies[ 0 ].setSrcStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
    theSubPassDependencies[ 0 ].setSrcAccessMask( vk::AccessFlagBits::eNone );
    theSubPassDependencies[ 0 ].setDstStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
    theSubPassDependencies[ 0 ].setDstAccessMask( vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite );

    auto theColorAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        inImageFormat,
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    );

    std::vector< vk::AttachmentDescription > theAttachmentDescriptions =
    {
        theColorAttachmentDescription
    };
    auto theRenderPassCreateInfo = vk::RenderPassCreateInfo
    (
        vk::RenderPassCreateFlags(),
        theAttachmentDescriptions.size(),
        theAttachmentDescriptions.data(),
        1,
        &theSubpassDescription,
        theSubPassDependencies.size(),
        theSubPassDependencies.data()
    );

    mRenderPass = mDevice.GetVkDevice().createRenderPass
    (
        theRenderPassCreateInfo
    );
}

vkt::RenderPass::~RenderPass()
{
    mDevice.GetVkDevice().destroyRenderPass( mRenderPass );
}

vk::RenderPass
vkt::RenderPass::GetVkRenderPass() const
{
    return mRenderPass;
}

void
vkt::RenderPass::Begin( vk::CommandBuffer inCommandBuffer, vk::Framebuffer inFrameBuffer, vk::Rect2D inRect, vk::ClearValue inClearValue )
{
    auto renderPassBeginInfo = vk::RenderPassBeginInfo
    (
        mRenderPass,
        inFrameBuffer,
        inRect,
        1,
        &inClearValue
    );
    inCommandBuffer.beginRenderPass( renderPassBeginInfo, vk::SubpassContents::eInline );
}
