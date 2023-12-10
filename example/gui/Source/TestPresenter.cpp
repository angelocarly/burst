#include "example/gui/TestPresenter.h"

#include "vkt/Device.h"
#include "vkt/ForwardDecl.h"
#include "vkt/GraphicsPipeline.h"
#include "vkt/RenderPass.h"
#include "vkt/Shader.h"
#include "vkt/ComputePipeline.h"

#include <imgui.h>

example::gui::TestPresenter::TestPresenter( burst::PresentContext const & inContext )
:
    mContext( inContext ),
    mComputeDescriptorSetLayout(
        vkt::DescriptorSetLayoutBuilder( mContext.mDevice )
        .AddLayoutBinding( 0, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eCompute )
        .Build( vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
    ),
    mGraphicsDescriptorSetLayout(
        vkt::DescriptorSetLayoutBuilder( mContext.mDevice )
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
        .Build( vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
    )
{
    // Image
    mImage = vkt::ImageFactory( mContext.mDevice ).CreateImage
    (
        mContext.mWidth,
        mContext.mHeight,
        vk::Format::eR32Uint,
        vk::ImageTiling::eOptimal,
        vk::ImageUsageFlagBits::eStorage | vk::ImageUsageFlagBits::eSampled | vk::ImageUsageFlagBits::eTransferDst,
        vma::AllocationCreateFlagBits::eDedicatedMemory,
        "Test Image",
        1
    );

    // Image layout
    auto commandBuffer = mContext.mDevice.BeginSingleTimeCommands();
    mImage->MemoryBarrier
    (
        commandBuffer,
        vk::ImageLayout::eGeneral,
        vk::AccessFlagBits::eNone,
        vk::AccessFlagBits::eShaderRead,
        vk::PipelineStageFlagBits::eAllCommands,
        vk::PipelineStageFlagBits::eComputeShader,
        vk::DependencyFlagBits::eByRegion
    );
    mContext.mDevice.EndSingleTimeCommands( commandBuffer );

    // Sampler
    mSampler = mContext.mDevice.GetVkDevice().createSampler
    (
        vk::SamplerCreateInfo
        (
            vk::SamplerCreateFlags(),
            vk::Filter::eNearest,
            vk::Filter::eNearest,
            vk::SamplerMipmapMode::eNearest,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            vk::SamplerAddressMode::eRepeat,
            0.0f,
            VK_FALSE,
            16.0f,
            VK_FALSE,
            vk::CompareOp::eAlways,
            0.0f,
            0.0f,
            vk::BorderColor::eIntOpaqueBlack,
            VK_FALSE
        )
    );

    // Image view
    mImageView = mContext.mDevice.GetVkDevice().createImageView
    (
        vk::ImageViewCreateInfo
        (
            vk::ImageViewCreateFlags(),
            mImage->GetVkImage(),
            vk::ImageViewType::e2D,
            vk::Format::eR32Uint,
            vk::ComponentMapping(),
            vk::ImageSubresourceRange
            (
                vk::ImageAspectFlagBits::eColor,
                0,
                1,
                0,
                1
            )
        )
    );

    // Compute shader
    auto computeShader = vkt::Shader::CreateVkShaderModule( mContext.mDevice, "resources/shaders/Compute.comp" );

    std::vector< vk::PushConstantRange > pushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eCompute,
            0,
            sizeof( PushConstants )
        )
    };

    mComputePipeline = vkt::ComputePipelineBuilder( mContext.mDevice )
        .SetComputeShader( computeShader )
        .SetDescriptorSetLayouts( mComputeDescriptorSetLayout )
        .SetPushConstants( pushConstants )
        .Build();

    mContext.mDevice.GetVkDevice().destroy( computeShader );

    // Present shader
    auto vertexShader = vkt::Shader::CreateVkShaderModule( mContext.mDevice, "resources/shaders/ScreenRect.vert" );
    auto fragmentShader = vkt::Shader::CreateVkShaderModule( mContext.mDevice, "resources/shaders/Sampler.frag" );

    mPipeline = vkt::GraphicsPipelineBuilder( mContext.mDevice )
        .SetDescriptorSetLayouts( mGraphicsDescriptorSetLayout )
        .SetVertexShader( vertexShader )
        .SetFragmentShader( fragmentShader )
        .SetRenderPass( mContext.mRenderPass->GetVkRenderPass() )
        .Build();

    mContext.mDevice.GetVkDevice().destroy( vertexShader );
    mContext.mDevice.GetVkDevice().destroy( fragmentShader );

}

example::gui::TestPresenter::~TestPresenter()
{
    mContext.mDevice.GetVkDevice().destroy( mImageView );
    mContext.mDevice.GetVkDevice().destroy( mSampler );
}

void
example::gui::TestPresenter::Compute( vk::CommandBuffer inCommandBuffer ) const
{
    // Reset image
    inCommandBuffer.clearColorImage
    (
        mImage->GetVkImage(),
        vk::ImageLayout::eGeneral,
        vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 0.0f } ),
        vk::ImageSubresourceRange
        (
            vk::ImageAspectFlagBits::eColor,
            0,
            1,
            0,
            1
        )
    );

    // Begin pipeline
    mComputePipeline->Bind( inCommandBuffer );

    // Push descriptor set
    auto imageInfo = vk::DescriptorImageInfo
    (
        mSampler,
        mImageView,
        vk::ImageLayout::eGeneral
    );

    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstBinding( 0 );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eStorageImage );
    theWriteDescriptorSet.setDescriptorCount( 1 );
    theWriteDescriptorSet.setPImageInfo( & imageInfo );

    mComputePipeline->BindPushDescriptorSet( inCommandBuffer, theWriteDescriptorSet );

    // Push constants
    auto time = std::chrono::duration_cast<std::chrono::microseconds>
    (
        std::chrono::system_clock::now().time_since_epoch()
    );
    inCommandBuffer.pushConstants
    (
        mComputePipeline->GetVkPipelineLayout(),
        vk::ShaderStageFlagBits::eCompute,
        0,
        sizeof( PushConstants ),
        & mPushConstants
    );

    inCommandBuffer.dispatch( mWorkGroups, 1, 1 );
}

void
example::gui::TestPresenter::Present( vk::CommandBuffer inCommandBuffer ) const
{
    // Draw screen rect
    mPipeline->Bind( inCommandBuffer );

    // Push descriptor set
    auto imageInfo = vk::DescriptorImageInfo
    (
        mSampler,
        mImageView,
        vk::ImageLayout::eGeneral
    );

    auto theWriteDescriptorSet = vk::WriteDescriptorSet();
    theWriteDescriptorSet.setDstBinding( 0 );
    theWriteDescriptorSet.setDstArrayElement( 0 );
    theWriteDescriptorSet.setDescriptorType( vk::DescriptorType::eCombinedImageSampler );
    theWriteDescriptorSet.setDescriptorCount( 1 );
    theWriteDescriptorSet.setPImageInfo( & imageInfo );

    mPipeline->BindPushDescriptorSet( inCommandBuffer, theWriteDescriptorSet );

    inCommandBuffer.draw( 3, 1, 0, 0 );
}

void
example::gui::TestPresenter::Update( float inDelta )
{
    ImGui::Begin( "My First Tool" );
    {
        ImGui::SliderFloat( "Time", & mPushConstants.mTime, 0.0f, 10000.0f );
        ImGui::SliderFloat( "Scale", & mPushConstants.mScale, 0.1f, 5.0f );
        ImGui::SliderFloat( "RayLength", & mPushConstants.mRayLength, 1.0f, 500.0f );
        ImGui::SliderInt( "Workgroups", & mWorkGroups, 0, 1000 );
    }
    ImGui::End();
}


