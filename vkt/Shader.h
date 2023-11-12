#ifndef Vkt_Shader_h
#define Vkt_Shader_h

#include "vkt/Device.h"

#include <filesystem>

namespace vkt
{
    class Shader
    {
        public:
            static vk::ShaderModule CreateVkShaderModule( const vkt::Device & inDevice, const std::filesystem::path & inPath );
    };
}

#endif
