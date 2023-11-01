#include "burst/Engine.h"

#include "burst/Window.h"
#include "vkt/Instance.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

burst::Engine::Engine( std::size_t inWidth, std::size_t inHeight, const char * inTitle )
:
    mWindow( inWidth, inHeight, inTitle ),
    mInstance( CreateInstance() )
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

vkt::Instance
burst::Engine::CreateInstance() const
{
    // Check glfw Vulkan support
    // Note: GLFW should be initialized before requesting Vulkan support
    if( glfwVulkanSupported() == GLFW_FALSE )
    {
        spdlog::get( "burst" )->error( "Vulkan not supported" );
        throw std::runtime_error( "Vulkan not supported" );
    }

    // Get the required extensions
    std::vector< const char * > requiredExtensions;
    std::uint32_t mExtensionCount = 0;
    auto glfwExtensions = glfwGetRequiredInstanceExtensions( &mExtensionCount );
    for( int i = 0; i < mExtensionCount; ++i )
    {
        requiredExtensions.push_back( glfwExtensions[ i ] );
    }

    return vkt::Instance( requiredExtensions );
}
