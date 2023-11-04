#ifndef Vkt_Image_h
#define Vkt_Image_h

#include <vulkan/vulkan.hpp>

namespace vkt
{
    class Image
    {
        public:
            Image();
            Image( vk::Image inImage );
            ~Image();

            vk::Image GetVkImage() const;
            vk::ImageLayout GetVkImageLayout() const;

        private:
            void SetImageLayout( vk::ImageLayout inImageLayout );

        private:
            vk::Image mImage;
            vk::ImageLayout mImageLayout;
    };
}

#endif
