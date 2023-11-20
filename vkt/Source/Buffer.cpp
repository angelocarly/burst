#include "vkt/Buffer.h"

#include "vkt/Device.h"

vkt::Buffer::Buffer( vkt::Device const & inDevice )
:
    mDevice( inDevice )
{
}

vkt::Buffer::Buffer( vkt::Device const & inDevice, vk::Buffer inBuffer, vma::Allocation inAllocation )
:
    mDevice( inDevice ),
    mBuffer( inBuffer ),
    mAllocation( inAllocation )
{
}

vkt::Buffer::~Buffer()
{
    if( mAllocation )
    {
        mDevice.GetVmaAllocator().destroyBuffer( mBuffer, mAllocation );
    }

    // Don't destroy images without allocation i.e. swapchain images
}

vk::Buffer
vkt::Buffer::GetVkBuffer() const
{
    return mBuffer;
}

void *
vkt::Buffer::MapMemory()
{
    return mDevice.GetVmaAllocator().mapMemory( mAllocation );
}

void
vkt::Buffer::UnMapMemory()
{
    mDevice.GetVmaAllocator().unmapMemory( mAllocation );
}

// =====================================================================================================================

vkt::BufferFactory::BufferFactory( vkt::Device const & inDevice )
:
    mDevice( inDevice )
{
}

vkt::BufferFactory::~BufferFactory()
{
}

vkt::BufferPtr
vkt::BufferFactory::CreateBuffer( vk::DeviceSize inSize, vk::BufferUsageFlags inUsageFlags, vma::AllocationCreateFlags inAllocationCreateFlags, const std::string & inAllocationName ) const
{
    auto bufferCreateInfo = vk::BufferCreateInfo
    (
        vk::BufferCreateFlags(),
        inSize,
        inUsageFlags
    );

    auto allocationCreateInfo = vma::AllocationCreateInfo
    (
        inAllocationCreateFlags,
        vma::MemoryUsage::eAuto
    );

    auto result = mDevice.GetVmaAllocator().createBuffer
    (
        bufferCreateInfo,
        allocationCreateInfo
    );
    return std::make_shared< vkt::Buffer >( mDevice, result.first, result.second );
}


