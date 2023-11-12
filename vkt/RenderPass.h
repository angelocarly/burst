#ifndef Vkt_RenderPass_h
#define Vkt_RenderPass_h

#include "vkt/Device.h"

#include <vulkan/vulkan.hpp>

namespace vkt
{
    class RenderPass
    {
        public:
            RenderPass( const vkt::Device & inDevice, vk::Format inImageFormat );
            ~RenderPass();

            vk::RenderPass GetVkRenderPass() const;

            void Begin( vk::CommandBuffer inCommandBuffer, vk::Framebuffer inFrameBuffer, vk::Rect2D inRect, vk::ClearValue inClearValue );

        private:
            vkt::Device const & mDevice;
            vk::RenderPass mRenderPass;
    };
}

#endif
