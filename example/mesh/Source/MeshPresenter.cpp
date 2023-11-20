/**
 * MeshPresenter.cpp
 *
 * @file	MeshPresenter.cpp
 * @author	Angelo Carly
 * @date	20/11/2023
 *
 * Copyright (c) 2023 Hybrid Software Development. All rights reserved.
 */

#include "example/mesh/MeshPresenter.h"

#include "burst/Presenter.h"

#include "vkt/RenderPass.h"
#include "vkt/DescriptorSetLayout.h"
#include "vkt/GraphicsPipeline.h"
#include "vkt/Shader.h"
#include "vkt/Mesh.h"

example::MeshPresenter::MeshPresenter( burst::PresentContext const & inContext, std::function< void( vk::CommandBuffer ) > inMeshCallback )
:
    mContext( inContext ),
    mMeshCallback( inMeshCallback )
{
    mGraphicsDescriptorSetLayout = vkt::DescriptorSetLayoutBuilder( inContext.mDevice )
        .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
        .Build();

    auto vertexShader = vkt::Shader::CreateVkShaderModule( inContext.mDevice, "resources/shaders/Mesh.vert" );
    auto fragmentShader = vkt::Shader::CreateVkShaderModule( inContext.mDevice, "resources/shaders/Mesh.frag" );

    mPipeline = vkt::GraphicsPipelineBuilder( inContext.mDevice )
        .SetDescriptorSetLayouts( mGraphicsDescriptorSetLayout )
        .SetRenderPass( inContext.mRenderPass->GetVkRenderPass() )
        .SetVertexShader( vertexShader )
        .SetFragmentShader( fragmentShader )
        .SetVertexInputBindingDescriptions( vkt::Vertex::GetVkVertexInputBindingDescriptions() )
        .SetVertexInputAttributeDescriptions( vkt::Vertex::GetVkVertexInputAttributeDescriptions() )
        .Build();

    inContext.mDevice.GetVkDevice().destroy( vertexShader );
    inContext.mDevice.GetVkDevice().destroy( fragmentShader );
}

example::MeshPresenter::~MeshPresenter()
{
    mContext.mDevice.GetVkDevice().waitIdle();
}

void
example::MeshPresenter::Compute( vk::CommandBuffer inCommandBuffer ) const
{

}

void
example::MeshPresenter::Present( vk::CommandBuffer inCommandBuffer ) const
{
    mPipeline->Bind( inCommandBuffer );

    mMeshCallback( inCommandBuffer );
}
