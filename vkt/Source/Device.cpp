#include "vkt/Device.h"

#include "vkt/PhysicalDevice.h"

#include <spdlog/spdlog.h>

vkt::Device::Device( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance )
:
    mDevice( CreateDevice( inPhysicalDevice ) ),
    mQueue( mDevice.getQueue( inPhysicalDevice.FindQueueFamilyIndices().graphicsFamilyIndex.value(), 0 ) ),
    mCommandPool( CreateCommandPool( inPhysicalDevice ) ),
    mAllocator( CreateAllocator( inPhysicalDevice, inInstance ) )
{
}

vkt::Device::~Device()
{

}

vk::Device
vkt::Device::CreateDevice( const vkt::PhysicalDevice & inPhysicalDevice )
{
    spdlog::get( "vkt" )->debug( "Initializing logical device." );

    vk::PhysicalDeviceFeatures deviceFeatures = vk::PhysicalDeviceFeatures();

    std::array< vk::DeviceQueueCreateInfo, 1 > deviceQueueCreateInfos;
    float queuePriority = 1.0f;
    deviceQueueCreateInfos[ 0 ] = vk::DeviceQueueCreateInfo
    (
        vk::DeviceQueueCreateFlags(),
        inPhysicalDevice.FindQueueFamilyIndices().graphicsFamilyIndex.value(),
        1,
        & queuePriority
    );

    auto requiredExtensions = inPhysicalDevice.GetDeviceExtensions();
    vk::DeviceCreateInfo theDeviceCreateInfo
    (
        vk::DeviceCreateFlags(),
        std::uint32_t( deviceQueueCreateInfos.size() ),
        deviceQueueCreateInfos.data(),
        0,
        nullptr,
        std::uint32_t( requiredExtensions.size() ),
        requiredExtensions.data(),
        & deviceFeatures
    );

    for( const char * theExtensionName : inPhysicalDevice.GetDeviceExtensions() )
    {
        spdlog::get( "vkt" )->debug( "- {}", theExtensionName );
    }

    return inPhysicalDevice.GetVkPhysicalDevice().createDevice( theDeviceCreateInfo );
}

vk::CommandPool
vkt::Device::CreateCommandPool( const vkt::PhysicalDevice & inPhysicalDevice )
{
    return mDevice.createCommandPool
    (
        vk::CommandPoolCreateInfo
        (
            vk::CommandPoolCreateFlagBits::eResetCommandBuffer,
            inPhysicalDevice.FindQueueFamilyIndices().graphicsFamilyIndex.value()
        )
    );
}

vk::Device
vkt::Device::GetVkDevice() const
{
    return mDevice;
}

vma::Allocator
vkt::Device::CreateAllocator( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance )
{
    vma::AllocatorCreateInfo theAllocatorInfo
    (
        vma::AllocatorCreateFlags(),
        inPhysicalDevice.GetVkPhysicalDevice(),
        mDevice
    );
    theAllocatorInfo.instance = inInstance.GetVkInstance();
    return vma::createAllocator( theAllocatorInfo );
}
