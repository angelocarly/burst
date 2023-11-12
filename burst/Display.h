#ifndef Burst_Display_h
#define Burst_Display_h

#include "burst/Window.h"

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

        public:
            /**
             * @brief Renders a frame to the display.
             * @param inComputeCallback A callback that will be called before the present callback. This is where you should record your compute commands.
             * @param inPresentCallback A callback that will be called during the frame's renderpass. This is where you should record your present commands. Don't start new renderpasses here.
             */
            void Render( std::function<void(vk::CommandBuffer const &)> inComputeCallback, std::function<void(vk::CommandBuffer const &)> inPresentCallback  );

        private:
            void InitializeCommandBuffers();
            void InitializeRenderPass();
            void InitializePipeline( vk::RenderPass inRenderPass );
            void InitializeFrameBuffers();

            vkt::Device const & mDevice;
            vkt::Swapchain mSwapchain;
            std::vector< vk::CommandBuffer > mCommandBuffers;

            vkt::RenderPassPtr mRenderPass;
            vk::ClearColorValue mClearColor;
            vk::ClearValue mClearValue;
            std::vector< vk::Framebuffer > mFramebuffers;

            vkt::DescriptorSetLayoutsPtr mDescriptorSetLayout;
            vkt::GraphicsPipelinePtr mPipeline;
    };
}

#endif
