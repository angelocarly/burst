#include "burst/Engine.h"

#include "burst/Window.h"
#include "burst/Utils.h"

#include "vkt/Instance.h"
#include "vkt/PhysicalDevice.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_beta.h>

namespace
{
    std::vector< const char * >
    GetInstanceExtensions( burst::VulkanConfig inVulkanConfig )
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

    std::vector< const char * >
    GetDeviceExtensions()
    {
        static std::vector< const char * > theExtensions;

        theExtensions.push_back( VK_KHR_SWAPCHAIN_EXTENSION_NAME );
        theExtensions.push_back( VK_KHR_PUSH_DESCRIPTOR_EXTENSION_NAME );

        if( burst::Utils::IsTargetApple() )
        {
            // Allow using a subset of the vulkan api
            theExtensions.push_back( VK_KHR_PORTABILITY_SUBSET_EXTENSION_NAME );
        }

        return theExtensions;
    }
}

burst::Engine::Engine( std::size_t inWidth, std::size_t inHeight, const char * inTitle, VulkanConfig inVulkanConfig )
:
    mWindow( inWidth, inHeight, inTitle ),
    mInstance( CreateInstance( inVulkanConfig ) ),
    mPhysicalDevice( mInstance, GetDeviceExtensions() ),
    mDevice( mPhysicalDevice, mInstance )
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
    auto requiredExtensions = GetInstanceExtensions( inVulkanConfig );

    return vkt::Instance( requiredExtensions );
}