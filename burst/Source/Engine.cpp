#include "burst/Engine.h"

#include "burst/Window.h"
#include "burst/Utils.h"

#include "vkt/Instance.h"
#include "vkt/PhysicalDevice.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_beta.h>

#include <chrono>

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
    mInstance( CreateInstance( inVulkanConfig ) ),
    mWindow( mInstance, inWidth, inHeight, inTitle ),
    mPhysicalDevice( mInstance, GetDeviceExtensions() ),
    mDevice( mPhysicalDevice, mInstance ),
    mDisplay( mDevice, mWindow ),
    mGui( mDevice, mWindow, mDisplay.GetSwapchain(), * mDisplay.GetPresentContext().mRenderPass )
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::stdout_color_mt("burst");

    spdlog::get( "burst" )->info( "Started engine" );

    mPreviousFrameTime = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    mPreviousSecond = mPreviousFrameTime;
}

burst::Engine::~Engine()
{
    spdlog::get( "burst" )->info( "Stopped engine" );
}

void
burst::Engine::Run()
{
    std::chrono::microseconds frameTime = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    std::size_t frameCount;

    while( !mWindow.ShouldClose() )
    {
        frameCount++;

        mPreviousFrameTime = frameTime;
        frameTime = std::chrono::duration_cast<std::chrono::microseconds>
        (
            std::chrono::system_clock::now().time_since_epoch()
        );
        float theFrameDuration = ( frameTime.count() - mPreviousFrameTime.count() ) % 10000000000 / 1000000.0f;

        mWindow.Poll();

        Update( theFrameDuration );

        mDisplay.Render( * this );

        float theSecondDuration = ( frameTime.count() - mPreviousSecond.count() ) % 10000000000 / 1000000.0f;
        if( theSecondDuration > 1 )
        {
            mPreviousSecond = frameTime;
            auto fps = frameCount;
            frameCount = 0;
            spdlog::get("burst")->info( "FPS: {}", fps );
        }
    }
}


vkt::Instance
burst::Engine::CreateInstance( VulkanConfig inVulkanConfig ) const
{
    glfwInit();
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

burst::PresentContext const &
burst::Engine::GetPresentContext()
{
    return mDisplay.GetPresentContext();
}

void
burst::Engine::Compute( vk::CommandBuffer inCommandBuffer ) const
{
    GetPresenter().Compute( inCommandBuffer );
    mGui.Compute( inCommandBuffer );
}

void
burst::Engine::Present( vk::CommandBuffer inCommandBuffer ) const
{
    GetPresenter().Present( inCommandBuffer );
    mGui.Present( inCommandBuffer );
}
