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
    class MultiPresenter
    :
        public Presenter
    {
        public:
            MultiPresenter( std::vector< Presenter * > inPresenters )
            :
                mPresenters( inPresenters )
            {
            }
            ~MultiPresenter() = default;

            void Compute( vk::CommandBuffer inCommandBuffer ) override
            {
                for( auto presenter : mPresenters )
                {
                    presenter->Compute( inCommandBuffer );
                }
            }
            void Present( vk::CommandBuffer inCommandBuffer ) override
            {
                for( auto presenter : mPresenters )
                {
                    presenter->Present( inCommandBuffer );
                }
            }

        private:
            std::vector< Presenter * > mPresenters;
    };

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
