#include "vkt/Shader.h"
#include "vkt/Device.h"

#include <vulkan/vulkan.hpp>

#include <filesystem>
#include <fstream>
#include <vector>

namespace
{
    std::vector< char > const
    ReadFile( std::filesystem::path const & inPath )
    {
        std::error_code error;

        if( !std::filesystem::exists( inPath ) )
        {
            std::string error = "Could not find shader: " + inPath.string();
            throw std::runtime_error( error );
        }

        std::vector< char > buffer( std::filesystem::file_size( inPath, error ) );

        std::ifstream theFile( inPath );
        theFile.read( buffer.data(), buffer.size() );

        return buffer;
    }
}

vk::ShaderModule
vkt::Shader::CreateVkShaderModule( vkt::Device const & inDevice, std::filesystem::path const & inPath )
{
    auto spvPath = inPath;
    spvPath.replace_extension( inPath.extension().string() + ".spv" );
    auto const shaderCode = ReadFile( spvPath );

    vk::ShaderModuleCreateInfo shaderModuleCreateInfo
    (
        vk::ShaderModuleCreateFlags(),
        shaderCode.size(),
        reinterpret_cast< uint32_t const * >( shaderCode.data() )
    );

    return inDevice.GetVkDevice().createShaderModule( shaderModuleCreateInfo );
}