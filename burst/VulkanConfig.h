#ifndef Burst_VulkanConfig_h
#define Burst_VulkanConfig_h

#include <vector>

namespace burst
{
    class VulkanConfig
    {
        public:
            VulkanConfig() = default;
            ~VulkanConfig() = default;

            std::vector< const char * > mInstanceExtensions;
    };

    class VulkanConfigBuilder
    {
        public:
            VulkanConfigBuilder() = default;
            ~VulkanConfigBuilder() = default;

            VulkanConfigBuilder & SetInstanceExtensions( std::vector< const char * > inRequiredInstanceExtensions );

            VulkanConfig Build() const;

        private:
            VulkanConfig mConfig;
    };
}

#endif
