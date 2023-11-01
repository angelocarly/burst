#ifndef Vkt_Instance_h
#define Vkt_Instance_h

#include <vulkan/vulkan.hpp>

namespace vkt
{
    class Instance
    {
        public:
            Instance( std::vector< const char * > inInstanceExtensions );
            ~Instance() = default;

        private:
            void CreateVulkanInstance( std::vector< const char * > inInstanceExtensions );
            void GetDebugUtilsMessengerCreateInfo( vk::DebugUtilsMessengerCreateInfoEXT & inCreateInfo ) const;
            std::vector< const char * > GetRequiredExtensions() const;

        private:
            vk::Instance mInstance;

            const std::vector< const char * > mValidationLayers =
            {
                "VK_LAYER_KHRONOS_validation"
            };
    };
}

#endif
