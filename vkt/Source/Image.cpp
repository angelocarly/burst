#include "vkt/Image.h"

#include "vkt/Device.h"

#include <vk_mem_alloc.hpp>

vkt::Image::Image( vkt::Device const & inDevice )
:
    mDevice( inDevice ),
    mImageLayout( vk::ImageLayout::eUndefined )
{
}

vkt::Image::Image( vkt::Device const & inDevice, vk::Image inImage, vma::Allocation inAllocation )
:
    mDevice( inDevice ),
    mImage( inImage ),
    mAllocation( inAllocation ),
    mImageLayout( vk::ImageLayout::eUndefined )
{
}

vkt::Image::~Image()
{
    if( mAllocation )
    {
        mDevice.GetVmaAllocator().destroyImage( mImage, mAllocation );
    }

    // Don't destroy images without allocation i.e. swapchain images
}

vk::Image
vkt::Image::GetVkImage() const
{
    return mImage;
}

vk::ImageLayout
vkt::Image::GetVkImageLayout() const
{
    return mImageLayout;
}

void
vkt::Image::SetImageLayout( vk::ImageLayout inImageLayout )
{
    mImageLayout = inImageLayout;
}

void
vkt::Image::MemoryBarrier
(
    vk::CommandBuffer inCommandBuffer,
    vk::ImageLayout inNewLayout,
    vk::AccessFlags inSrcAccessMask,
    vk::AccessFlags inDstAccessMask,
    vk::PipelineStageFlags inSrcStageMask,
    vk::PipelineStageFlags inDstStageMask,
    vk::DependencyFlags inDependencyFlags
)
{
    vk::ImageSubresourceRange theImageSubResourceRange
    (
        vk::ImageAspectFlagBits::eColor,
        0,
        1,
        0,
        1
    );
    vk::ImageMemoryBarrier theImageMemoryBarrier
    (
        inSrcAccessMask,
        inDstAccessMask,
        GetVkImageLayout(),
        inNewLayout,
        VK_QUEUE_FAMILY_IGNORED,
        VK_QUEUE_FAMILY_IGNORED,
        GetVkImage(),
        theImageSubResourceRange,
        nullptr
    );
    inCommandBuffer.pipelineBarrier
    (
        inSrcStageMask,
        inDstStageMask,
        inDependencyFlags,
        nullptr,
        nullptr,
        theImageMemoryBarrier
    );

    mImageLayout = inNewLayout;
}

// ============================================== Image ================================================================

vkt::ImageFactory::ImageFactory( vkt::Device const & inDevice )
:
    mDevice( inDevice )
{
}

vkt::ImageFactory::~ImageFactory()
{
}

vkt::ImagePtr
vkt::ImageFactory::CreateImage( std::size_t inWidth, std::size_t inHeight, vk::Format inFormat, vk::ImageTiling inTiling, vk::ImageUsageFlags inUsageFlags, vma::AllocationCreateFlags inAllocationCreateFlags, const std::string & inAllocationName, std::size_t inArrayLayers ) const
{
    auto imageAllocation = mDevice.GetVmaAllocator().createImage
    (
        vk::ImageCreateInfo
        (
            vk::ImageCreateFlags(),
            vk::ImageType::e2D,
            inFormat,
            vk::Extent3D( inWidth, inHeight, 1 ),
            1,
            1,
            vk::SampleCountFlagBits::e1,
            vk::ImageTiling::eOptimal,
            inUsageFlags,
            vk::SharingMode::eExclusive,
            1,
            0,
            vk::ImageLayout::eUndefined
        ),
        vma::AllocationCreateInfo
        (
            inAllocationCreateFlags,
            vma::MemoryUsage::eAuto
        )
    );

    return std::make_shared< vkt::Image >( mDevice, imageAllocation.first, imageAllocation.second );
}
