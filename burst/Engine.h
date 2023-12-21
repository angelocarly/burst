#ifndef Burst_Engine_h
#define Burst_Engine_h

#include "burst/Display.h"
#include "burst/VulkanConfig.h"
#include "burst/Window.h"

#include "vkt/Device.h"
#include "vkt/Instance.h"
#include "vkt/PhysicalDevice.h"
#include "GuiPresenter.h"

#include <chrono>

namespace burst
{
    class Engine
    :
        public Presenter
    {
        public:
            Engine( std::size_t inWidth, std::size_t inHeight, const char * inTitle, VulkanConfig inVulkanConfig = VulkanConfig() );
            ~Engine();

        public:
            void Run();
            burst::PresentContext const & GetPresentContext();

            virtual void Update( float inDelta ) = 0;
            virtual burst::Presenter & GetPresenter() const = 0;

        public:
            void Compute( vk::CommandBuffer inCommandBuffer ) const;
            void Present( vk::CommandBuffer inCommandBuffer ) const;

        private:
            vkt::Instance CreateInstance( VulkanConfig inVulkanConfig ) const;

            vkt::Instance mInstance;
            vkt::PhysicalDevice mPhysicalDevice;
            vkt::Device mDevice;
            burst::Window mWindow;
            burst::Display mDisplay;

            std::chrono::microseconds mPreviousFrameTime;
            std::chrono::microseconds mPreviousSecond;

            burst::GuiPresenter mGui;
    };
}

#endif
