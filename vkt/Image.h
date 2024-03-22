#ifndef Vkt_Image_h
#define Vkt_Image_h

#include "vkt/Device.h"

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace vkt
{
    class Image
    {
        public:
            Image( vkt::Device const & inDevice );
            Image( vkt::Device const & inDevice, vk::Image inImage, vk::Extent2D inExtent, vma::Allocation inAllocation = nullptr );
            ~Image();

            vk::Image GetVkImage() const;
            vk::ImageLayout GetVkImageLayout() const;
            std::size_t GetWidth() const;
            std::size_t GetHeight() const;

            void MemoryBarrier
            (
                vk::CommandBuffer inCommandBuffer,
                vk::ImageLayout inNewLayout,
                vk::AccessFlags inSrcAccessMask,
                vk::AccessFlags inDstAccessMask,
                vk::PipelineStageFlags inSrcStageMask,
                vk::PipelineStageFlags inDstStageMask,
                vk::DependencyFlags inDependencyFlags
            );

        private:
            void SetImageLayout( vk::ImageLayout inImageLayout );

        private:
            vkt::Device const & mDevice;

            vk::Image mImage;
            vma::Allocation mAllocation;
            vk::ImageLayout mImageLayout;

            std::size_t mWidth;
            std::size_t mHeight;
    };

    class ImageFactory
    {
        public:
            ImageFactory( vkt::Device const & inDevice );
            ~ImageFactory();

            ImagePtr CreateImage( std::size_t inWidth, std::size_t inHeight, vk::Format inFormat, vk::ImageTiling inTiling, vk::ImageUsageFlags inUsageFlags, vma::AllocationCreateFlags inAllocationCreateFlags, const std::string & inAllocationName, std::size_t inArrayLayers ) const;
            ImagePtr CreateColorLookupImage( std::size_t inWidth, vk::Format inFormat, vk::ImageTiling inTiling, vk::ImageUsageFlags inUsageFlags, vma::AllocationCreateFlags inAllocationCreateFlags, const std::string & inAllocationName, std::size_t inArrayLayers ) const;

            void DestroyImage( Image const & inImage ) const;

        private:
            vkt::Device const & mDevice;
    };
}

#endif
