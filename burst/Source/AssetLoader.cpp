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

    // srgb correction
    std::vector< std::uint8_t > thePixels = std::vector< std::uint8_t >( pixels, pixels + texWidth * texHeight * channelCount );
    for( std::size_t i = 0; i < texWidth * texHeight * 4; i += texChannels )
    {
        thePixels[ i + 0 ] = pow( thePixels[ i + 0 ] / 255.0f, 2.2f ) * 255;
        thePixels[ i + 1 ] = pow( thePixels[ i + 1 ] / 255.0f, 2.2f ) * 255;
        thePixels[ i + 2 ] = pow( thePixels[ i + 2 ] / 255.0f, 2.2f ) * 255;
//        thePixels[ i + 3 ] = pow( thePixels[ i ], 2.2f );
    }
    ImageAsset theImage = { ( std::size_t ) texWidth, ( std::size_t ) texHeight, std::move( thePixels ) };

    // Free buffer
    stbi_image_free( pixels );

    return theImage;
}

void
burst::AssetLoader::SaveImage( burst::ImageAsset const & inImage, std::filesystem::path inPath )
{
    auto texWidth = inImage.mWidth;
    auto texHeight = inImage.mHeight;
    auto channelCount = 4;
    // srgb correction
    std::vector< std::uint8_t > thePixels = std::vector< std::uint8_t >( inImage.mPixels.data(), inImage.mPixels.data() + texWidth * texHeight * channelCount );
    for( std::size_t i = 0; i < texWidth * texHeight * 4; i += channelCount )
    {
        thePixels[ i + 0 ] = pow( thePixels[ i + 0 ] / 255.0f, 1.0f / 2.2f ) * 255;
        thePixels[ i + 1 ] = pow( thePixels[ i + 1 ] / 255.0f, 1.0f / 2.2f ) * 255;
        thePixels[ i + 2 ] = pow( thePixels[ i + 2 ] / 255.0f, 1.0f / 2.2f ) * 255;
//        thePixels[ i + 3 ] = pow( thePixels[ i ], 2.2f );
    }
    ImageAsset theImage = { ( std::size_t ) texWidth, ( std::size_t ) texHeight, std::move( thePixels ) };

    stbi_write_png
    (
        inPath.c_str(),
        theImage.mWidth,
        theImage.mHeight,
        4,
        theImage.mPixels.data(),
        theImage.mWidth * channelCount * sizeof( std::uint8_t )
    );
}
