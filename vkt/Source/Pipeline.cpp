#include "vkt/Pipeline.h"

#include "vkt/Device.h"

#include <tuple>

vkt::Pipeline::Pipeline( vkt::Device const & inDevice, vk::PipelineBindPoint inPipelineBindPoint, std::tuple< vk::Pipeline, vk::PipelineLayout > inPipeline )
:
    mDevice( inDevice ),
    mPipelineBindPoint( inPipelineBindPoint ),
    mPipeline( std::get< 0 >( inPipeline ) ),
    mPipelineLayout( std::get< 1 >( inPipeline ) )
{
}

vkt::Pipeline::~Pipeline()
{
    mDevice.GetVkDevice().destroy( mPipelineLayout );
    mDevice.GetVkDevice().destroyPipeline( mPipeline );
}

void
vkt::Pipeline::Bind( vk::CommandBuffer inCommandBuffer )
{
    inCommandBuffer.bindPipeline( mPipelineBindPoint, mPipeline );
}

void
vkt::Pipeline::BindPushDescriptorSet( vk::CommandBuffer inCommandBuffer, vk::WriteDescriptorSet inWriteDescriptorSet )
{
    VkPipelineBindPoint vkPipelineBindPoint;
    switch( mPipelineBindPoint )
    {
        case vk::PipelineBindPoint::eCompute:
            vkPipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_COMPUTE;
            break;
        case vk::PipelineBindPoint::eGraphics:
            vkPipelineBindPoint = VkPipelineBindPoint::VK_PIPELINE_BIND_POINT_GRAPHICS;
            break;
        default:
            throw std::runtime_error( "vkt::Pipeline::BindPushDescriptorSet: Unsupported pipeline bind point." );
    }

    PFN_vkCmdPushDescriptorSetKHR pfnVkCmdPushDescriptorSetKhr = reinterpret_cast< PFN_vkCmdPushDescriptorSetKHR >( mDevice.GetVkDevice().getProcAddr( "vkCmdPushDescriptorSetKHR" ) );
    pfnVkCmdPushDescriptorSetKhr
    (
        inCommandBuffer,
        vkPipelineBindPoint,
        mPipelineLayout,
        0,
        1,
        reinterpret_cast< const VkWriteDescriptorSet * >(& inWriteDescriptorSet)
    );
}

void
vkt::Pipeline::PushConstants( vk::CommandBuffer inCommandBuffer, vk::ShaderStageFlags inShaderStageFlags, std::uint32_t inOffset, std::uint32_t inSize, const void * inData )
{
    inCommandBuffer.pushConstants( mPipelineLayout, inShaderStageFlags, inOffset, inSize, inData );
}

vk::Pipeline
vkt::Pipeline::GetVkPipeline() const
{
    return mPipeline;
}

vk::PipelineLayout
vkt::Pipeline::GetVkPipelineLayout() const
{
    return mPipelineLayout;
}


