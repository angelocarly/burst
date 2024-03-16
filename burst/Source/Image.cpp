#include "burst/Image.h"

#include <glm/glm.hpp>

#include <memory>
#include <utility>

burst::Image::Image( vkt::Device const & inDevice, std::shared_ptr< vkt::Image > inImage, vk::Sampler inSampler, vk::ImageView inImageView )
:
    mDevice( inDevice ),
    mImage( std::move( inImage ) ),
    mSampler( inSampler ),
    mImageView( inImageView )
{
}

burst::Image::~Image()
{
    mDevice.GetVkDevice().destroySampler( mSampler );
    mDevice.GetVkDevice().destroyImageView( mImageView );
}

vk::DescriptorImageInfo
burst::Image::CreateDescriptorImageInfo() const
{
    return { mSampler, mImageView, mImage->GetVkImageLayout() };
}

std::shared_ptr< burst::Image >
burst::ImageFactory::Create( vkt::Device const & inDevice, glm::ivec2 inSize, vk::ImageLayout inLayout )
{
    // Image
    auto image = vkt::ImageFactory( inDevice ).CreateImage
    (
        inSize.x,
        inSize.y,
        vk::Format::eR8G8B8A8Unorm,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst | vk::ImageUsageFlagBits::eTransferSrc,
        vma::AllocationCreateFlagBits::eDedicatedMemory,
        "Test Image",
        1
    );

    auto commandBuffer = inDevice.BeginSingleTimeCommands();
    {
        // Image layout
        image->MemoryBarrier
        (
            commandBuffer,
            inLayout,
            vk::AccessFlagBits::eNone,
            vk::AccessFlagBits::eShaderRead,
            vk::PipelineStageFlagBits::eAllCommands,
            vk::PipelineStageFlagBits::eComputeShader,
            vk::DependencyFlagBits::eByRegion
        );
    }
    inDevice.EndSingleTimeCommands( commandBuffer );

    // Sampler
    vk::Sampler sampler = inDevice.GetVkDevice().createSampler
    (
        vk::SamplerCreateInfo
        (
            vk::SamplerCreateFlags(),
            vk::Filter::eNearest,
            vk::Filter::eNearest,
            vk::SamplerMipmapMode::eLinear,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            VK_FALSE,
            16.0f,
            VK_FALSE,
            vk::CompareOp::eAlways,
            0.0f,
            0.0f,
            vk::BorderColor::eIntOpaqueBlack,
            VK_FALSE
        )
    );

    // Image view
    vk::ImageView imageView = inDevice.GetVkDevice().createImageView
    (
        vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            image->GetVkImage(),
            vk::ImageViewType::e2D,
            vk::Format::eR8G8B8A8Unorm,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange
            (
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        )
    );

    return std::make_shared< burst::Image >( inDevice, image, sampler, imageView );
}