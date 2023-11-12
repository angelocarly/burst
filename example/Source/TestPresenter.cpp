#include "example/TestPresenter.h"

#include "vkt/Device.h"
#include "vkt/ForwardDecl.h"
#include "vkt/GraphicsPipeline.h"
#include "vkt/RenderPass.h"
#include "vkt/Shader.h"

example::TestPresenter::TestPresenter( burst::PresentContext const & inContext )
:
    mContext( inContext ),
    mDescriptorSetLayout(
        vkt::DescriptorSetLayoutBuilder( mContext.mDevice )
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
        .Build( vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
    )
{
    auto vertexShader = vkt::Shader::CreateVkShaderModule( mContext.mDevice, "resources/shaders/ScreenRect.vert" );
    auto fragmentShader = vkt::Shader::CreateVkShaderModule( mContext.mDevice, "resources/shaders/Sampler.frag" );

    mPipeline = vkt::GraphicsPipelineBuilder( mContext.mDevice )
        .SetDescriptorSetLayouts( mDescriptorSetLayout )
        .SetVertexShader( vertexShader )
        .SetFragmentShader( fragmentShader )
        .SetRenderPass( mContext.mRenderPass->GetVkRenderPass() )
        .Build();

    mContext.mDevice.GetVkDevice().destroy( vertexShader );
    mContext.mDevice.GetVkDevice().destroy( fragmentShader );

}

example::TestPresenter::~TestPresenter()
{
    mContext.mDevice.GetVkDevice().waitIdle();
}

void
example::TestPresenter::Compute( vk::CommandBuffer inCommandBuffer ) const
{

}

void
example::TestPresenter::Present( vk::CommandBuffer inCommandBuffer ) const
{
    // Draw screen rect
    mPipeline->Bind( inCommandBuffer );
    inCommandBuffer.draw( 3, 1, 0, 0 );
}


