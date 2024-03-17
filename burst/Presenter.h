#ifndef Burst_Presenter_h
#define Burst_Presenter_h

#include "vkt/Device.h"

#include <vulkan/vulkan.hpp>

namespace burst
{
    struct PresentContext
    {
        vkt::Device const & mDevice;
        vkt::RenderPassPtr mRenderPass;
        std::size_t mWidth;
        std::size_t mHeight;
    };

    class Presenter
    {
        public:
            /**
             * @brief process any command buffer events
             */
            virtual void Compute( vk::CommandBuffer inCommandBuffer ) = 0;

            /**
             * @brief present to the current frame. The corresponding renderpass is already bound.
             */
            virtual void Present( vk::CommandBuffer inCommandBuffer ) = 0;
    };
}

#endif
