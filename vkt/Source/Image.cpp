#include "vkt/Image.h"

vkt::Image::Image( vk::Image inImage )
:
    mImage( inImage )
{
}

vkt::Image::Image()
{
}

vkt::Image::~Image()
{
}

vk::Image
vkt::Image::GetVkImage() const
{
    return mImage;
}

vk::ImageLayout
vkt::Image::GetVkImageLayout() const
{
    return mImageLayout;
}

void
vkt::Image::SetImageLayout( vk::ImageLayout inImageLayout )
{
    mImageLayout = inImageLayout;
}

