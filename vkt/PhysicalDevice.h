#ifndef Vkt_PhysicalDevice_h
#define Vkt_PhysicalDevice_h

#include "vkt/Instance.h"

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
            vk::PhysicalDevice GetVkPhysicalDevice() const;
            QueueFamilyIndices FindQueueFamilyIndices() const;
            std::vector< const char * > GetDeviceExtensions() const;

        private:
            vk::PhysicalDevice mPhysicalDevice;
            std::vector< const char * > mDeviceExtensions;
    };
}

#endif
