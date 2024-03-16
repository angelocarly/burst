#ifndef Burst_Image_h
#define Burst_Image_h

#include "vkt/Image.h"

#include <glm/glm.hpp>
#include <vulkan/vulkan.hpp>

namespace burst
{
    class Image
    {
        public:
            Image( vkt::Device const & inDevice, std::shared_ptr< vkt::Image > inImage, vk::Sampler inSampler, vk::ImageView inImageView );
            ~Image();

            std::shared_ptr< vkt::Image > GetImage() const { return mImage; }
            vk::Sampler GetSampler() const { return mSampler; }
            vk::ImageView GetImageView() const { return mImageView; }

            friend class ImageFactory;

        private:
            vkt::Device const & mDevice;
            std::shared_ptr< vkt::Image > mImage;
            vk::Sampler mSampler;
            vk::ImageView mImageView;
    };

    class ImageFactory
    {
        public:
            std::shared_ptr< Image > Create( vkt::Device const & inDevice, glm::ivec2 inSize, vk::ImageLayout inLayout );
    };
}

#endif