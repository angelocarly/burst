#ifndef Burst_Display_h
#define Burst_Display_h

#include "burst/Window.h"

#include "vkt/Swapchain.h"

namespace burst
{
    class Display
    {
        public:
            Display( vkt::Device const & inDevice, burst::Window const & inWindow );
            ~Display();

        public:
            void Render( std::function<void(vk::CommandBuffer const &)> inRenderCallback );

        private:
            void InitializeCommandBuffers();
            void InitializeRenderPass();
            void InitializePipeline( vk::RenderPass inRenderPass );
            void InitializeFrameBuffers();
            vk::RenderPassBeginInfo CreateRenderPassBeginInfo( std::size_t inFrameIndex );

            vkt::Device const & mDevice;
            vkt::Swapchain mSwapchain;
            std::vector< vk::CommandBuffer > mCommandBuffers;

            vk::RenderPass mRenderPass;
            vk::ClearColorValue mClearColor;
            vk::ClearValue mClearValue;
            std::vector< vk::Framebuffer > mFramebuffers;
    };
}

#endif
