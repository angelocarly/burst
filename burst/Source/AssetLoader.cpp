#include "burst/AssetLoader.h"

#include <filesystem>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>

burst::ImageAsset
burst::AssetLoader::LoadImage( std::filesystem::path inPath )
{
    std::size_t channelCount = 4;

    int texWidth, texHeight, texChannels;
    stbi_uc * pixels = stbi_load( inPath.c_str(), & texWidth, & texHeight, & texChannels, channelCount );

    if( !pixels )
    {
        throw std::runtime_error( "failed to load texture image!" );
    }

    std::vector< std::uint8_t > thePixels = std::vector< std::uint8_t >( pixels, pixels + texWidth * texHeight * channelCount );
    ImageAsset theImage = { ( std::size_t ) texWidth, ( std::size_t ) texHeight, std::move( thePixels ) };

    // Free buffer
    stbi_image_free( pixels );

    return theImage;

}

void
burst::AssetLoader::SaveImage( burst::ImageAsset const & inImage, std::filesystem::path inPath )
{
    std::size_t channelCount = 4;
    stbi_write_png
    (
        inPath.c_str(),
        inImage.mWidth,
        inImage.mHeight,
        4,
        inImage.mPixels.data(),
        inImage.mWidth * channelCount * sizeof( std::uint8_t )
    );
}
