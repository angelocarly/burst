#ifndef Burst_Engine_h
#define Burst_Engine_h

#include "burst/VulkanConfig.h"
#include "burst/Window.h"

#include "vkt/Device.h"
#include "vkt/Instance.h"
#include "vkt/PhysicalDevice.h"

namespace burst
{
    class Engine
    {
        public:
            Engine( std::size_t inWidth, std::size_t inHeight, const char * inTitle, VulkanConfig inVulkanConfig = VulkanConfig() );
            ~Engine();

        public:
            void Run() const;

        private:
            vkt::Instance CreateInstance( VulkanConfig inVulkanConfig ) const;
            vkt::PhysicalDevice CreatePhysicalDevice( const vkt::Instance & inInstance ) const;

            burst::Window mWindow;
            vkt::Instance mInstance;
            vkt::PhysicalDevice mPhysicalDevice;
            vkt::Device mDevice;
    };
}

#endif
