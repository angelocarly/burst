#ifndef Vkt_PhysicalDevice_h
#define Vkt_PhysicalDevice_h

#include "Instance.h"

#include <vulkan/vulkan.hpp>

#include <optional>

namespace vkt
{
    struct QueueFamilyIndices
    {
        std::optional< std::uint32_t > graphicsFamilyIndex;

        bool IsComplete()
        {
            return graphicsFamilyIndex.has_value();
        }
    };

    class PhysicalDevice
    {
        public:
            explicit PhysicalDevice( const vkt::Instance & inInstance, std::vector< const char * > inDeviceExtensions );
            ~PhysicalDevice();

        public:
            vk::PhysicalDevice GetVkPhysicalDevice();
            QueueFamilyIndices FindQueueFamilyIndices();

        private:
            vk::PhysicalDevice mPhysicalDevice;
    };
}

#endif
