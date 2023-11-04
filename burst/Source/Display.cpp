#include "burst/Display.h"

burst::Display::Display( const vkt::Device & inDevice, const burst::Window & inWindow )
:
    mDevice( inDevice ),
    mSwapchain( inDevice, inWindow.GetSurface() )
{
    InitializeCommandBuffers();
    InitializeRenderPass();
    InitializeFrameBuffers();
}

burst::Display::~Display()
{
    mDevice.GetVkDevice().destroy( mRenderPass );
    for( auto framebuffer : mFramebuffers )
    {
        mDevice.GetVkDevice().destroy( framebuffer );
    }
}

void
burst::Display::Render( std::function<void( vk::CommandBuffer const & )> inRenderCallback )
{
    std::uint32_t theFrameIndex = mSwapchain.RetrieveNextImage();

    // Begin the current frame's draw commands
    auto theCommandBuffer = mCommandBuffers[ theFrameIndex ];
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        inRenderCallback( theCommandBuffer );
    }
    theCommandBuffer.end();

    mSwapchain.SubmitCommandBuffer( theFrameIndex, theCommandBuffer );
}

void
burst::Display::InitializeCommandBuffers()
{
    auto theCommandBufferCount = mSwapchain.GetImageCount();
    mCommandBuffers.resize( theCommandBufferCount );
    for( int i = 0; i < theCommandBufferCount; i++ )
    {
        mCommandBuffers[ i ] = mDevice.GetVkDevice().allocateCommandBuffers
        (
            vk::CommandBufferAllocateInfo( mDevice.GetVkCommandPool(), vk::CommandBufferLevel::ePrimary, 1 )
        ).front();
    }
}

void
burst::Display::InitializeRenderPass()
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
        mSwapchain.GetImageFormat(),
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

    mClearColor = vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 1.0f } );
    mClearValue = vk::ClearValue( mClearColor );
}

void
burst::Display::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain.GetImageViews();
    mFramebuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFramebuffers.size(); i++)
    {
        std::array< vk::ImageView, 1 > theAttachments =
        {
            theSwapchainImageViews[ i ]
        };
        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            theAttachments.size(),
            theAttachments.data(),
            mSwapchain.GetExtent().width,
            mSwapchain.GetExtent().height,
            1
        );
        mFramebuffers[i] = mDevice.GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
    }
}
