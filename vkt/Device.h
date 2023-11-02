#ifndef Vkt_Device_h
#define Vkt_Device_h

#include <vulkan/vulkan.hpp>

#include "vkt/PhysicalDevice.h"

namespace vkt
{
    class Device
    {
        public:
            explicit Device( const vkt::PhysicalDevice & inPhysicalDevice );
            ~Device();

        public:
            vk::Device GetVkDevice() const;

        private:
            vk::Device CreateDevice( const vkt::PhysicalDevice & inPhysicalDevice );
            vk::CommandPool CreateCommandPool( const vkt::PhysicalDevice & inPhysicalDevice );

        private:
            vk::Device mDevice;
            vk::Queue mQueue;
            vk::CommandPool mCommandPool;
    };
}

#endif
