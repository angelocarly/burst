#include "vkt/PhysicalDevice.h"

#include <spdlog/spdlog.h>

#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_beta.h>

namespace
{
    bool
    CheckExtensionSupport( vk::PhysicalDevice inDevice, std::vector< const char * > inRequiredExtensions )
    {
        std::vector< vk::ExtensionProperties > theExtensions = inDevice.enumerateDeviceExtensionProperties();
        for( const char * theRequiredExtension : inRequiredExtensions )
        {
            bool hasExtension = false;
            for( vk::ExtensionProperties theExtension : theExtensions )
            {
                if( strcmp( theExtension.extensionName, theRequiredExtension ) == 0 )
                {
                    hasExtension = true;
                    break;
                }
            }

            if( !hasExtension ) return false;
        }

        return true;
    }

    vkt::QueueFamilyIndices FindQueueFamilyIndices( vk::PhysicalDevice inDevice )
    {
        vkt::QueueFamilyIndices theQueueFamilyIndices {};

        int index = 0;
        for( vk::QueueFamilyProperties theQueueFamilyProperty: inDevice.getQueueFamilyProperties() )
        {
            if( theQueueFamilyProperty.queueFlags & vk::QueueFlagBits::eGraphics )
            {
                theQueueFamilyIndices.graphicsFamilyIndex = index;
            }

            index++;
        }

        return theQueueFamilyIndices;
    }
}

vkt::PhysicalDevice::PhysicalDevice( const vkt::Instance & inInstance, std::vector< const char * > inDeviceExtensions )
:
    mDeviceExtensions( inDeviceExtensions )
{
    spdlog::get( "vkt" )->debug( "Initializing physical device:" );

    std::vector< vk::PhysicalDevice > thePotentialDevices = inInstance.GetVkInstance().enumeratePhysicalDevices();
    for( vk::PhysicalDevice thePotentialDevice: thePotentialDevices )
    {
        if(
            CheckExtensionSupport( thePotentialDevice, inDeviceExtensions )
            && ::FindQueueFamilyIndices( thePotentialDevice ).IsComplete() )
        {
            mPhysicalDevice = thePotentialDevice;
            return;
        }
    }

    spdlog::get( "vkt" )->error( "Could not find a suitable physical device." );
    std::exit( 1 );
}

vkt::PhysicalDevice::~PhysicalDevice()
{

}

vk::PhysicalDevice
vkt::PhysicalDevice::GetVkPhysicalDevice() const
{
    return mPhysicalDevice;
}

std::vector< const char * >
vkt::PhysicalDevice::GetDeviceExtensions() const
{
    return mDeviceExtensions;
}

vkt::QueueFamilyIndices
vkt::PhysicalDevice::FindQueueFamilyIndices() const
{
    return ::FindQueueFamilyIndices( mPhysicalDevice );
}
