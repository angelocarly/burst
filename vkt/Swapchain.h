#ifndef Vkt_Swapchain_h
#define Vkt_Swapchain_h

#include "vkt/Device.h"
#include "vkt/Image.h"

namespace vkt
{
    class Swapchain
    {
        public:
            struct FrameInfo
            {
                std::size_t mCurrentFrame;
                std::size_t mImageIndex;
            };
        public:
            Swapchain( const vkt::Device & inDevice, const vk::SurfaceKHR & inSurface, vk::Extent2D inExtent );
            ~Swapchain();

            FrameInfo RetrieveNextImage() const;
            void SubmitCommandBuffer( std::uint32_t inImageIndex, vk::CommandBuffer inCommandBuffer );

            std::size_t GetMinImageCount() const { return 3; };
            std::size_t GetImageCount() const { return mSwapchainImages.size(); };
            vk::Format GetImageFormat() const { return vk::Format::eB8G8R8A8Srgb; };
            vk::ColorSpaceKHR GetColorSpace() const { return vk::ColorSpaceKHR::eSrgbNonlinear; };
            std::vector< vk::ImageView > const & GetImageViews() const { return mSwapchainImageViews; };
            std::vector< vkt::ImagePtr > const & GetImages() const { return mSwapchainImages; };
            vk::Extent2D GetExtent() const;

        private:
            vk::SwapchainKHR CreateSwapchain( const vkt::Device & inDevice, const vk::SurfaceKHR & inSurface, vk::Extent2D inExtent ) const;
            void PresentImage( uint32_t inImageIndex, vk::Semaphore const & inWaitSemaphore ) const;

            const vkt::Device & mDevice;
            vk::SwapchainKHR mSwapchain;
            std::vector< vkt::ImagePtr > mSwapchainImages;
            std::vector< vk::ImageView > mSwapchainImageViews;
            vk::Extent2D mExtent;

            std::uint32_t mCurrentFrame = 0;
            std::vector< vk::Fence > mCommandBufferExecutedFence;
            std::vector< vk::Semaphore > mImageAcquiredSemaphores;
            std::vector< vk::Semaphore > mBufferExecutedSemaphore;
            std::vector< bool > mFrameWasRendered;

            void InitializeSwapchainImages();
            void InitializeSynchronizationObjects();
    };
}

#endif
