#include "vkt/Swapchain.h"

#include "vkt/Image.h"

#include <spdlog/spdlog.h>

vkt::Swapchain::Swapchain( const vkt::Device & inDevice, const vk::SurfaceKHR & inSurface )
:
    mDevice( inDevice ),
    mExtent( PollExtent( inDevice, inSurface ) ),
    mSwapchain( CreateSwapchain( inDevice, inSurface ) )
{
    InitializeSwapchainImages();
    InitializeSynchronizationObjects();
}

vkt::Swapchain::~Swapchain()
{
    mDevice.GetVkDevice().waitForFences( mCommandBufferExecutedFence, true, UINT64_MAX );
    mDevice.GetVkDevice().waitIdle();

    for( int i = 0; i < mSwapchainImageViews.size(); i++ )
    {
        mDevice.GetVkDevice().destroy( mSwapchainImageViews[ i ] );
    }

    for( auto theFence : mCommandBufferExecutedFence )
    {
        mDevice.GetVkDevice().destroy( theFence );
    }
    for( auto theSemaphore : mImageAcquiredSemaphores )
    {
        mDevice.GetVkDevice().destroy( theSemaphore );
    }
    for( auto theSemaphore : mBufferExecutedSemaphore )
    {
        mDevice.GetVkDevice().destroy( theSemaphore );
    }

    mDevice.GetVkDevice().destroy( mSwapchain );
}

vk::Extent2D
vkt::Swapchain::PollExtent( const vkt::Device & inDevice, const vk::SurfaceKHR & inSurface ) const
{
    auto physicalDevice = inDevice.GetPhysicalDevice();
    auto surfaceCapabilities = physicalDevice.GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( inSurface );
    return surfaceCapabilities.currentExtent;
}

vk::SwapchainKHR
vkt::Swapchain::CreateSwapchain( const vkt::Device & inDevice, const vk::SurfaceKHR & inSurface ) const
{
    // Supported hardware capabilities and formats
    auto physicalDevice = inDevice.GetPhysicalDevice();
    auto surfaceCapabilities = physicalDevice.GetVkPhysicalDevice().getSurfaceCapabilitiesKHR( inSurface );
    auto surfaceFormats = physicalDevice.GetVkPhysicalDevice().getSurfaceFormatsKHR( inSurface );
    auto presentModes = physicalDevice.GetVkPhysicalDevice().getSurfacePresentModesKHR( inSurface );
    auto extent = surfaceCapabilities.currentExtent;

    const uint32_t graphicsFamilyIndex = inDevice.GetPhysicalDevice().FindQueueFamilyIndices().graphicsFamilyIndex.value();

    // Verify whether the surface supports the colorformat/colorspace
    bool hasFormat = false;
    for( auto surfaceFormat : surfaceFormats )
    {
        if( surfaceFormat.format == vk::Format:: eB8G8R8A8Srgb && surfaceFormat.colorSpace == vk::ColorSpaceKHR::eSrgbNonlinear )
        {
            hasFormat = true;
            break;
        }
    }
    if( !hasFormat )
    {
        spdlog::get( "vkt" )->error
        (
            "The surface doesn't support the requested format ({:?}) with colorspace ({:?})",
            to_string( GetImageFormat() ),
            to_string( GetColorSpace() )
        );
        std::exit( 1 );
    }

    vk::SwapchainCreateInfoKHR swapchainCreateInfoKhr
    (
        vk::SwapchainCreateFlagsKHR(),
        inSurface,
        3,
        GetImageFormat(),
        GetColorSpace(),
        extent,
        1,
        vk::ImageUsageFlagBits::eColorAttachment,
        vk::SharingMode::eExclusive,
        1,
        &graphicsFamilyIndex,
        vk::SurfaceTransformFlagBitsKHR::eIdentity,
        vk::CompositeAlphaFlagBitsKHR::eOpaque,
        vk::PresentModeKHR::eFifo,
        true,
        nullptr
    );
    return inDevice.GetVkDevice().createSwapchainKHR( swapchainCreateInfoKhr );
}

void
vkt::Swapchain::InitializeSwapchainImages()
{
    auto swapchainImages = mDevice.GetVkDevice().getSwapchainImagesKHR( mSwapchain );
    for( auto swapchainImage : swapchainImages )
    {
        mSwapchainImages.emplace_back( std::make_shared< vkt::Image >( mDevice, swapchainImage ) );
    }

    // Transition the images to presentable layout
    auto commandBuffer = mDevice.BeginSingleTimeCommands();
    {
        for( auto & image : mSwapchainImages )
        {
            image->MemoryBarrier
            (
                commandBuffer,
                vk::ImageLayout::eColorAttachmentOptimal,
                vk::AccessFlagBits::eNone,
                vk::AccessFlagBits::eColorAttachmentWrite,
                vk::PipelineStageFlagBits::eTopOfPipe,
                vk::PipelineStageFlagBits::eColorAttachmentOutput,
                vk::DependencyFlagBits::eByRegion
            );
        }
    }
    mDevice.EndSingleTimeCommands( commandBuffer );

    mSwapchainImageViews.resize( mSwapchainImages.size() );

    for ( size_t i = 0; i < mSwapchainImageViews.size(); i++)
    {
        auto imageViewCreateInfo = vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mSwapchainImages[ i ]->GetVkImage(),
            vk::ImageViewType::e2D,
            GetImageFormat(),
            vk::ComponentMapping
            (
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity,
                vk::ComponentSwizzle::eIdentity
            ),
            vk::ImageSubresourceRange
            (
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        );

        mSwapchainImageViews[ i ] = mDevice.GetVkDevice().createImageView( imageViewCreateInfo );
    }
}

void
vkt::Swapchain::InitializeSynchronizationObjects()
{
    mFrameWasRendered.resize( mSwapchainImages.size() );

    mCommandBufferExecutedFence.resize( mSwapchainImages.size() );
    for( int i = 0; i < mCommandBufferExecutedFence.size(); ++i )
    {
        auto fenceCreateInfo = vk::FenceCreateInfo
        (
            vk::FenceCreateFlagBits::eSignaled // Mark the first image as signaled
        );
        mCommandBufferExecutedFence[ i ] = mDevice.GetVkDevice().createFence( fenceCreateInfo );
    }

    mImageAcquiredSemaphores.resize( mSwapchainImages.size() );
    for( int i = 0; i < mImageAcquiredSemaphores.size(); ++i )
    {
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo();
        mImageAcquiredSemaphores[ i ] = mDevice.GetVkDevice().createSemaphore( semaphoreCreateInfo );
    }

    mBufferExecutedSemaphore.resize( mSwapchainImages.size() );
    for( int i = 0; i < mBufferExecutedSemaphore.size(); ++i )
    {
        auto semaphoreCreateInfo = vk::SemaphoreCreateInfo( vk::SemaphoreCreateFlags() );
        mBufferExecutedSemaphore[ i ] = mDevice.GetVkDevice().createSemaphore( semaphoreCreateInfo );
    }
}

std::uint32_t
vkt::Swapchain::RetrieveNextImage() const
{
    while( vk::Result::eTimeout == mDevice.GetVkDevice().waitForFences( mCommandBufferExecutedFence[ mCurrentFrame ], VK_TRUE, UINT64_MAX ));
    mDevice.GetVkDevice().resetFences( mCommandBufferExecutedFence[ mCurrentFrame ] );

    // Block until a new image is acquired
    uint32_t imageIndex = 0;
    mDevice.GetVkDevice().acquireNextImageKHR
    (
        mSwapchain,
        UINT64_MAX,
        // The semaphore to signal when the image is acquired
        mImageAcquiredSemaphores[ mCurrentFrame ],
        nullptr,
        & imageIndex
    );

    // Reset the fence
    mDevice.GetVkDevice().resetFences( mCommandBufferExecutedFence[ mCurrentFrame ] );

    return imageIndex;
}

void
vkt::Swapchain::SubmitCommandBuffer( std::uint32_t inImageIndex, vk::CommandBuffer inCommandBuffer )
{
    vk::PipelineStageFlags theDstStageFlags = vk::PipelineStageFlagBits::eColorAttachmentOutput;
    mDevice.GetQueue().submit
    (
        vk::SubmitInfo
        (
            1,
            // Wait for the swapchain image to be acquired
            & mImageAcquiredSemaphores[ mCurrentFrame ],
            & theDstStageFlags,
            1,
            & inCommandBuffer,
            1,
            // Signal the semaphore when the command buffer has finished executing
            & mBufferExecutedSemaphore[ mCurrentFrame ]
        ),
        // Fence to trigger when the command buffer has finished executing
        mCommandBufferExecutedFence[ mCurrentFrame ]
    );

    // Display the swapchain image
    PresentImage( inImageIndex, mBufferExecutedSemaphore[ mCurrentFrame ] );

    mCurrentFrame = ( mCurrentFrame + 1 ) % mSwapchainImages.size();
}

/**
 * Present a swapchain image
 * NOTE: During debugging, it is possible that a frame is seemingly skipped and the same image is rendered twice.
 * It is unclear why this happens, when taking about five seconds between frames, the issue does not occur.
 * The issue only seems to happen on Mac for now.
 * @param inImageIndex the image to render
 * @param inWaitSemaphore semaphore to trigger before submission
 */
void
vkt::Swapchain::PresentImage( uint32_t inImageIndex, vk::Semaphore const & inWaitSemaphore ) const
{
    // Submit an image for presentation
    auto thePresentInfo = vk::PresentInfoKHR
    (
        inWaitSemaphore,
        mSwapchain,
        inImageIndex
    );

    auto result = mDevice.GetQueue().presentKHR( thePresentInfo );
    if( result != vk::Result::eSuccess && result != vk::Result::eSuboptimalKHR )
    {
        // TODO: recreate the swapchain
        throw std::runtime_error( "Error presenting swapchain" );
    }
}

vk::Extent2D
vkt::Swapchain::GetExtent() const
{
    return mExtent;
}
