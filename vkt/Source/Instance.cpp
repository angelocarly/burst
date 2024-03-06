#define VMA_IMPLEMENTATION
#include "vk_mem_alloc.hpp"

#include "vkt/Instance.h"

#include <vulkan/vulkan.hpp>
#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include <filesystem>

VkResult
vkCreateDebugUtilsMessengerEXT(VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT* pCreateInfo, const VkAllocationCallbacks* pAllocator, VkDebugUtilsMessengerEXT* pDebugMessenger)
{
    auto func = (PFN_vkCreateDebugUtilsMessengerEXT) vkGetInstanceProcAddr(instance, "vkCreateDebugUtilsMessengerEXT");
    if ( func != nullptr )
    {
        return func( instance, pCreateInfo, pAllocator, pDebugMessenger );
    } else
    {
        return VK_ERROR_EXTENSION_NOT_PRESENT;
    }
}

void
vkDestroyDebugUtilsMessengerEXT(VkInstance instance, VkDebugUtilsMessengerEXT debugMessenger, const VkAllocationCallbacks* pAllocator)
{
    auto func = ( PFN_vkDestroyDebugUtilsMessengerEXT ) vkGetInstanceProcAddr(instance, "vkDestroyDebugUtilsMessengerEXT");
    if ( func != nullptr )
    {
        func( instance, debugMessenger, pAllocator );
    }
}

static VKAPI_ATTR VkBool32 VKAPI_CALL DebugCallback
(
    VkDebugUtilsMessageSeverityFlagBitsEXT messageSeverity,
    VkDebugUtilsMessageTypeFlagsEXT,
    const VkDebugUtilsMessengerCallbackDataEXT * pCallbackData,
    void *
)
{
    switch( messageSeverity )
    {
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT:
            spdlog::get( "vkt" )->debug( "Validation layer: {}", pCallbackData->pMessage );
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_INFO_BIT_EXT:
            spdlog::get( "vkt" )->info( "Validation layer: {}", pCallbackData->pMessage );
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT:
            spdlog::get( "vkt" )->warn( "Validation layer: {}", pCallbackData->pMessage );
            break;
        case VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT:
            spdlog::get( "vkt" )->error( "Validation layer: {}", pCallbackData->pMessage );
            break;
        default:
            throw std::runtime_error( "Unknown message severity" );
    }
    return VK_FALSE;
}

// ---------------------------------------------------------------------------------------------------------------------

vkt::Instance::Instance( std::vector< const char * > inInstanceExtensions )
{
    auto test = std::filesystem::path( "." );
    auto a = absolute( test );
    // Register the logger
    spdlog::stdout_color_mt("vkt");

    // Setup Vulkan
    CreateVulkanInstance( inInstanceExtensions );
}

vkt::Instance::~Instance()
{
    spdlog::get( "vkt" )->debug( "Destroying instance" );

    mInstance.destroy();
}

void
vkt::Instance::GetDebugUtilsMessengerCreateInfo( vk::DebugUtilsMessengerCreateInfoEXT & inCreateInfo ) const
{
    inCreateInfo.flags = vk::DebugUtilsMessengerCreateFlagsEXT();
    inCreateInfo.messageSeverity = vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError;
    inCreateInfo.messageType = vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance;
    inCreateInfo.pfnUserCallback = &DebugCallback;
}

std::vector< const char * >
vkt::Instance::GetRequiredExtensions() const
{
    std::vector<const char*> requiredExtensions;

    // Mac OS X support
    requiredExtensions.emplace_back( VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME );

    // Provide debug information
    requiredExtensions.emplace_back( VK_EXT_DEBUG_UTILS_EXTENSION_NAME );

    return requiredExtensions;
}

void
vkt::Instance::CreateVulkanInstance( std::vector< const char * > inInstanceExtensions )
{
    auto applicationInfo = vk::ApplicationInfo
    (
        "VKT",
        VK_MAKE_VERSION( 1, 0, 0 ),
        "VKT",
        VK_MAKE_VERSION( 1, 0, 0 ),
        VK_API_VERSION_1_3
    );

    vk::DebugUtilsMessengerCreateInfoEXT theDebugUtilsMessengerCreateInfo;
    GetDebugUtilsMessengerCreateInfo( theDebugUtilsMessengerCreateInfo );

    auto theCreateFlagBits = vk::InstanceCreateFlagBits();
    theCreateFlagBits = vk::InstanceCreateFlagBits::eEnumeratePortabilityKHR;

    auto requiredExtensions = GetRequiredExtensions();
    for( auto & extension : inInstanceExtensions )
    {
        requiredExtensions.emplace_back( extension );
    }

    auto createInfo = vk::InstanceCreateInfo
    (
        theCreateFlagBits,
        &applicationInfo,
        std::uint32_t( mValidationLayers.size() ),
        mValidationLayers.data(),
        std::uint32_t( requiredExtensions.size() ),
        requiredExtensions.data(),
        &theDebugUtilsMessengerCreateInfo
    );

    mInstance = vk::createInstance( createInfo );
}

vk::Instance
vkt::Instance::GetVkInstance() const
{
    return mInstance;
}
