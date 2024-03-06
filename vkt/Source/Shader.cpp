#include "vkt/Shader.h"
#include "vkt/Device.h"

#include <vulkan/vulkan.hpp>

#include <filesystem>
#include <fstream>
#include <vector>
#include <spdlog/spdlog.h>

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

namespace
{
    bool
    IsTargetApple()
    {
#ifdef __APPLE__
        return true;
#else
        return false;
#endif
    }

    std::string GetGlslValidator()
    {
        if( IsTargetApple() )
        {
            return "/usr/local/bin/glslangValidator";
        }
        else
        {
            // linux
            return "/usr/bin/glslangValidator";
        }
    }
}

std::string
vkt::Shader::CompileShader( std::filesystem::path inPath )
{
    // First compile using glslc
    std::stringstream command;
    command << GetGlslValidator() << " --target-env vulkan1.0 -V " << inPath << " -o " << inPath << ".spv";

    spdlog::info( "Running command: {}", command.str() );

    std::array<char, 128> buffer{};
    std::string result;
    std::unique_ptr< FILE, decltype(&pclose) > pipe( popen( command.str().c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    spdlog::info( "Compiled shader output: {}", result );

    return result;
}
