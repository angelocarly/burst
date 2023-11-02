#ifndef Vkt_Device_h
#define Vkt_Device_h

#include "vkt/PhysicalDevice.h"

#include <vulkan/vulkan.hpp>

#define VMA_IMPLEMENTATION
#include <vk_mem_alloc.hpp>

namespace vkt
{
    class Device
    {
        public:
            explicit Device( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance );
            ~Device();

        public:
            vk::Device GetVkDevice() const;

        private:
            vk::Device CreateDevice( const vkt::PhysicalDevice & inPhysicalDevice ) const;
            vk::CommandPool CreateCommandPool( const vkt::PhysicalDevice & inPhysicalDevice ) const;
            vma::Allocator CreateAllocator( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance ) const;

        private:
            vk::Device mDevice;
            vk::Queue mQueue;
            vk::CommandPool mCommandPool;
            vma::Allocator mAllocator;
    };
}

#endif
