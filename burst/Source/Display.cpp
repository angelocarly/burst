#include <spdlog/spdlog.h>
#include "burst/Display.h"

#include "burst/Presenter.h"

#include "vkt/DescriptorSetLayout.h"
#include "vkt/FrameBuffer.h"
#include "vkt/RenderPass.h"

burst::Display::Display( const vkt::Device & inDevice, const burst::Window & inWindow )
:
    mDevice( inDevice ),
    mSwapchain( inDevice, inWindow.GetSurface() ),
    mRenderPass( std::make_shared< vkt::RenderPass >( mDevice, mSwapchain.GetImageFormat() ) ),
    mPresentContext(
        burst::PresentContext
        {
            mDevice,
            mRenderPass,
            mSwapchain.GetExtent().width,
            mSwapchain.GetExtent().height
        }
    )

{
    InitializeCommandBuffers();
    InitializeFrameBuffers();
}

burst::Display::~Display()
{
    spdlog::get( "vkt" )->debug( "Destroying display" );

    // Wait until all rendering is done
    mDevice.GetVkDevice().waitIdle();
}

void
burst::Display::Render( burst::Presenter & inPresenter )
{
    std::uint32_t theFrameIndex = mSwapchain.RetrieveNextImage();

    // Begin the current frame's draw commands
    auto commandBuffer = mCommandBuffers[ theFrameIndex ];
    commandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        auto image = mSwapchain.GetImages()[ theFrameIndex ];

        /*
         * Process compute commands
         */
        inPresenter.Compute( commandBuffer );

        mRenderPass->Begin( commandBuffer, mFramebuffers[ theFrameIndex ]->GetVkFramebuffer(), vk::Rect2D( vk::Offset2D( 0, 0 ), mSwapchain.GetExtent() ), mClearValue );
        {
            // Set dynamic state
            commandBuffer.setViewport( 0, vk::Viewport( 0.0f, 0.0f, mSwapchain.GetExtent().width, mSwapchain.GetExtent().height, 0.0f, 1.0f ) );
            commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), mSwapchain.GetExtent() ) );

            inPresenter.Present( commandBuffer );

        }
        commandBuffer.endRenderPass();
    }
    commandBuffer.end();

    mSwapchain.SubmitCommandBuffer( theFrameIndex, commandBuffer );
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
burst::Display::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain.GetImageViews();
    mFramebuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFramebuffers.size(); i++)
    {
        std::vector< vk::ImageView > theAttachments =
        {
            theSwapchainImageViews[ i ]
        };
        mFramebuffers[i] = std::make_shared< vkt::FrameBuffer >
        (
            mDevice,
            mRenderPass,
            theAttachments,
            mSwapchain.GetExtent().width,
            mSwapchain.GetExtent().height
        );
    }
}

burst::PresentContext const &
burst::Display::GetPresentContext() const
{
    return mPresentContext;
}

const vkt::Swapchain &
burst::Display::GetSwapchain()
{
    return mSwapchain;
}
