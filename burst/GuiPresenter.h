#ifndef Burst_GuiPresenter_h
#define Burst_GuiPresenter_h

#include "vkt/Device.h"

#include "burst/Presenter.h"
#include "burst/Window.h"

#include "vkt/Swapchain.h"

#include <imgui.h>

namespace burst
{
    class GuiPresenter
    :
        public Presenter
    {
        public:
            GuiPresenter( const vkt::Device & inDevice, const burst::Window & inWindow, const vkt::Swapchain & inSwapchain, const vkt::RenderPass & inRenderPass );
            ~GuiPresenter();

            void Compute( vk::CommandBuffer inCommandBuffer );
            void Present( vk::CommandBuffer inCommandBuffer );

        private:
            static void CheckVkResult( VkResult inResult );
            void InitializeImGui( const burst::Window & inWindow, const vkt::Swapchain & inSwapchain, const vkt::RenderPass & inRenderPass );
            void InitializeDescriptorPool();
            void SetImGuiStyle( ImGuiIO & io );

            vkt::Device const & mDevice;
            vk::DescriptorPool mDescriptorPool;
    };
}

#endif
