#ifndef Burst_Display_h
#define Burst_Display_h

#include "burst/Window.h"

#include "burst/Presenter.h"

#include "vkt/DescriptorSetLayout.h"
#include "vkt/Swapchain.h"
#include "vkt/GraphicsPipeline.h"
#include "vkt/RenderPass.h"

namespace burst
{
    /**
     * @brief Wraps a window's swapchain and provides a simple interface for render to it.
     */
    class Display
    {
        public:
            Display( vkt::Device const & inDevice, burst::Window const & inWindow );
            ~Display();

            burst::PresentContext const & GetPresentContext() const;

        public:
            /**
             * @brief Renders a frame to the display.
             */
            void Render( burst::Presenter const & inPresenter );

        private:
            void InitializeCommandBuffers();
            void InitializeFrameBuffers();

            vkt::Device const & mDevice;
            vkt::Swapchain mSwapchain;
            std::vector< vk::CommandBuffer > mCommandBuffers;

            vkt::RenderPassPtr mRenderPass;
            vk::ClearColorValue mClearColor;
            vk::ClearValue mClearValue;
            std::vector< vkt::FrameBufferPtr > mFramebuffers;

            PresentContext mPresentContext;
    };
}

#endif
