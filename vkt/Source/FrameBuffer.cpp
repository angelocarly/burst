#include "vkt/FrameBuffer.h"

#include "vkt/RenderPass.h"

vkt::FrameBuffer::FrameBuffer( const vkt::Device & inDevice, vkt::RenderPassPtr inRenderPass, std::vector< vk::ImageView > inAttachments, std::size_t inWidth, std::size_t inHeight )
:
    mDevice( inDevice )
{
    auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
    (
        vk::FramebufferCreateFlags(),
        inRenderPass->GetVkRenderPass(),
        inAttachments.size(),
        inAttachments.data(),
        inWidth,
        inHeight,
        1
    );
    mFramebuffer = mDevice.GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
}

vkt::FrameBuffer::~FrameBuffer()
{
    mDevice.GetVkDevice().destroyFramebuffer( mFramebuffer );
}
