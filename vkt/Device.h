#ifndef Vkt_Device_h
#define Vkt_Device_h

#include "vkt/ForwardDecl.h"
#include "vkt/PhysicalDevice.h"

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace vkt
{
    class Device
    {
        private:
//            Device( const Device& ) = delete; // non construction-copyable

        public:
            explicit Device( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance );
            Device& operator=( const Device& )
            {
                return *this;
            }

            ~Device();

        public:
            vk::Device GetVkDevice() const;
            vk::Queue GetQueue() const;
            vk::CommandPool GetVkCommandPool() const;
            vkt::PhysicalDevice GetPhysicalDevice() const;
            vk::CommandBuffer BeginSingleTimeCommands() const;
            void EndSingleTimeCommands( vk::CommandBuffer & inCommandBuffer ) const;

            std::vector< vkt::Image > GetSwapchainImages( vk::SwapchainKHR & inSwapchain ) const;

            void ImageMemoryBarrier( vk::CommandBuffer inCommandBuffer, Image inImage, vk::ImageLayout inNewLayout, vk::AccessFlags inSrcAccessMask, vk::AccessFlags inDstAccessMask, vk::PipelineStageFlags inSrcStageMask, vk::PipelineStageFlags inDstStageMask, vk::DependencyFlags inDependencyFlags ) const;

        private:
            vk::Device CreateDevice( const vkt::PhysicalDevice & inPhysicalDevice ) const;
            vk::CommandPool CreateCommandPool( const vkt::PhysicalDevice & inPhysicalDevice ) const;
            vma::Allocator CreateAllocator( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance ) const;

        private:
            vkt::PhysicalDevice mPhysicalDevice;
            vk::Device mDevice;
            vk::Queue mQueue;
            vk::CommandPool mCommandPool;
            vma::Allocator mAllocator;

    };
}

#endif
