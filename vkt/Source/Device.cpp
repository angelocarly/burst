#include "vkt/Device.h"

#include <spdlog/spdlog.h>

vkt::Device::Device( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance )
:
    mInstance( inInstance ),
    mPhysicalDevice( inPhysicalDevice ),
    mDevice( CreateDevice( inPhysicalDevice ) ),
    mQueue( mDevice.getQueue( inPhysicalDevice.FindQueueFamilyIndices().graphicsFamilyIndex.value(), 0 ) ),
    mCommandPool( CreateCommandPool( inPhysicalDevice ) ),
    mAllocator( CreateAllocator( inPhysicalDevice, inInstance ) )
{
}

vkt::Device::~Device()
{
    spdlog::get( "vkt" )->debug( "Destroying device" );

    mDevice.waitIdle();
    mAllocator.destroy();
    mDevice.destroyCommandPool( mCommandPool );
    mDevice.destroy();
}

vk::Device
vkt::Device::CreateDevice( const vkt::PhysicalDevice & inPhysicalDevice ) const
{
    spdlog::get( "vkt" )->debug( "Initializing logical device." );

    auto deviceFeatures = vk::PhysicalDeviceFeatures();

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
vkt::Device::CreateCommandPool( const vkt::PhysicalDevice & inPhysicalDevice ) const
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

vma::Allocator
vkt::Device::CreateAllocator( const vkt::PhysicalDevice & inPhysicalDevice, const vkt::Instance & inInstance ) const
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

vkt::Instance const &
vkt::Device::GetInstance() const
{
    return mInstance;
}

vk::Device
vkt::Device::GetVkDevice() const
{
    return mDevice;
}

vkt::PhysicalDevice
vkt::Device::GetPhysicalDevice() const
{
    return mPhysicalDevice;
}

vk::Queue
vkt::Device::GetQueue() const
{
    return mQueue;
}

vk::CommandPool
vkt::Device::GetVkCommandPool() const
{
    return mCommandPool;
}

vma::Allocator
vkt::Device::GetVmaAllocator() const
{
    return mAllocator;
}


// =====================================================================================================================

vk::CommandBuffer
vkt::Device::BeginSingleTimeCommands() const
{
    auto theCommandBuffer =  mDevice.allocateCommandBuffers
    (
        vk::CommandBufferAllocateInfo( mCommandPool, vk::CommandBufferLevel::ePrimary, 1 )
    ).front();
    theCommandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    return theCommandBuffer;
}

void
vkt::Device::EndSingleTimeCommands( vk::CommandBuffer & inCommandBuffer ) const
{
    inCommandBuffer.end();

    // Submit command buffer and wait until completion
    vk::Fence theFence = mDevice.createFence( vk::FenceCreateInfo() );
    mQueue.submit( vk::SubmitInfo( 0, nullptr, nullptr, 1, &inCommandBuffer ), theFence );
    while( vk::Result::eTimeout == mDevice.waitForFences( theFence, VK_TRUE, UINT64_MAX ));
    mDevice.destroyFence( theFence );

    mDevice.freeCommandBuffers( mCommandPool, inCommandBuffer );
}

void
vkt::Device::WaitIdle() const
{
    mDevice.waitIdle();
}

