#include "burst/Engine.h"

#include "burst/Window.h"
#include "vkt/Instance.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

burst::Engine::Engine( std::size_t inWidth, std::size_t inHeight, const char * inTitle, VulkanConfig inVulkanConfig )
:
    mWindow( inWidth, inHeight, inTitle ),
    mInstance( CreateInstance( inVulkanConfig ) )
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::stdout_color_mt("burst");

    spdlog::get( "burst" )->info( "Started engine" );
}

burst::Engine::~Engine()
{
    spdlog::get( "burst" )->info( "Stopped engine" );
}

void
burst::Engine::Run() const
{
    while( !mWindow.ShouldClose() )
    {
        mWindow.Poll();
    }
}

namespace
{
    std::vector< const char * > GetRequiredExtensions( burst::VulkanConfig inVulkanConfig )
    {
        std::vector< const char * > requiredExtensions;

        // Load the glfw extensions
        std::uint32_t glfwExtensionCount = 0;
        auto glfwExtensions = glfwGetRequiredInstanceExtensions( &glfwExtensionCount );
        for( int i = 0; i < glfwExtensionCount; ++i )
        {
            requiredExtensions.push_back( glfwExtensions[ i ] );
        }

        // Append the requested extensions
        requiredExtensions.insert( requiredExtensions.end(), inVulkanConfig.mInstanceExtensions.begin(), inVulkanConfig.mInstanceExtensions.end() );

        return requiredExtensions;
    }
}

vkt::Instance
burst::Engine::CreateInstance( VulkanConfig inVulkanConfig ) const
{
    // Check glfw Vulkan support
    // Note: GLFW should be initialized before requesting Vulkan support
    if( glfwVulkanSupported() == GLFW_FALSE )
    {
        spdlog::get( "burst" )->error( "Vulkan not supported" );
        throw std::runtime_error( "Vulkan not supported" );
    }

    // Setup the required instance extensions
    auto requiredExtensions = GetRequiredExtensions( inVulkanConfig );

    return vkt::Instance( requiredExtensions );
}
