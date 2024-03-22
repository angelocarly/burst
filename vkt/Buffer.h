#ifndef Vkt_Buffer_h
#define Vkt_Buffer_h

#include "vkt/ForwardDecl.h"

#include <vk_mem_alloc.hpp>
#include <vulkan/vulkan.hpp>

namespace vkt
{
    class Buffer
    {
        public:
            Buffer( vkt::Device const & inDevice );
            Buffer( vkt::Device const & inDevice, vk::Buffer inBuffer, vk::DeviceSize inSize, vma::Allocation inAllocation = nullptr );
            ~Buffer();

            vk::Buffer GetVkBuffer() const;

            void MemoryBarrier
            (
                vk::CommandBuffer inCommandBuffer,
                vk::AccessFlags inSrcAccessMask,
                vk::AccessFlags inDstAccessMask,
                vk::PipelineStageFlags inSrcStageMask,
                vk::PipelineStageFlags inDstStageMask,
                vk::DependencyFlags inDependencyFlags
            );

            void * MapMemory();
            void UnMapMemory();

            vk::DeviceSize GetSize();
            vk::DescriptorBufferInfo GetVkDescriptorBufferInfo();

        private:
            vkt::Device const & mDevice;

            vk::Buffer mBuffer;
            vma::Allocation mAllocation;
            vk::DeviceSize mSize;
    };

    class BufferFactory
    {
        public:
            BufferFactory( vkt::Device const & inDevice );
            ~BufferFactory();

            BufferPtr CreateBuffer( vk::DeviceSize inSize, vk::BufferUsageFlags inUsageFlags, vma::AllocationCreateFlags inAllocationCreateFlags, const std::string & inAllocationName ) const;

        private:
            vkt::Device const & mDevice;
    };
}

#endif
