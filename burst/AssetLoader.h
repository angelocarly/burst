#ifndef Burst_AssetLoader_h
#define Burst_AssetLoader_h

#include <filesystem>
#include <vector>

namespace burst
{
    struct ImageAsset
    {
        std::size_t mWidth;
        std::size_t mHeight;
        std::vector< std::uint8_t > mPixels;
    };

    class AssetLoader
    {
        public:
            static burst::ImageAsset LoadImage( std::filesystem::path inPath );
            static void SaveImage( burst::ImageAsset const & inImage, std::filesystem::path inPath );
    };
}

#endif
