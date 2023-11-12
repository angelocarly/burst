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
            virtual void Compute( vk::CommandBuffer inCommandBuffer ) const = 0;
            virtual void Present( vk::CommandBuffer inCommandBuffer ) const = 0;
    };
}

#endif
