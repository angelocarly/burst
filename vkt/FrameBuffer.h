#ifndef Vkt_FrameBuffer_h
#define Vkt_FrameBuffer_h

#include "vkt/Device.h"

namespace vkt
{
    class FrameBuffer
    {
        public:
            FrameBuffer( const vkt::Device & inDevice, vkt::RenderPassPtr inRenderPass, std::vector< vk::ImageView > inAttachments, std::size_t inWidth, std::size_t inHeight );
            ~FrameBuffer();

            vk::Framebuffer GetVkFramebuffer() const { return mFramebuffer; }

        private:
            vkt::Device const & mDevice;
            vk::Framebuffer mFramebuffer;
    };
}

#endif
