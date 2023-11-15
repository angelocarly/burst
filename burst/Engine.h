#ifndef Burst_Engine_h
#define Burst_Engine_h

#include "burst/Display.h"
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
            void Run();
            burst::PresentContext const & GetPresentContext();

            virtual void Update() const = 0;
            virtual burst::Presenter & GetPresenter() const = 0;

        private:
            vkt::Instance CreateInstance( VulkanConfig inVulkanConfig ) const;

            vkt::Instance mInstance;
            burst::Window mWindow;
            vkt::PhysicalDevice mPhysicalDevice;
            vkt::Device mDevice;
            burst::Display mDisplay;

    };
}

#endif
