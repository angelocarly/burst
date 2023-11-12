#include "burst/Display.h"

#include "vkt/DescriptorSetLayout.h"
#include "vkt/FrameBuffer.h"
#include "vkt/GraphicsPipeline.h"
#include "vkt/RenderPass.h"
#include "vkt/Shader.h"

burst::Display::Display( const vkt::Device & inDevice, const burst::Window & inWindow )
:
    mDevice( inDevice ),
    mSwapchain( inDevice, inWindow.GetSurface() ),
    mDescriptorSetLayout(
        vkt::DescriptorSetLayoutBuilder( mDevice )
            .AddLayoutBinding( 0, vk::DescriptorType::eCombinedImageSampler, vk::ShaderStageFlagBits::eFragment )
            .Build( vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR )
    )

{
    InitializeCommandBuffers();
    mRenderPass = std::make_shared< vkt::RenderPass >( mDevice, mSwapchain.GetImageFormat() );
    InitializeFrameBuffers();
    InitializePipeline( mRenderPass->GetVkRenderPass() );
}

burst::Display::~Display()
{
    // Wait until all rendering is done
    mDevice.GetVkDevice().waitIdle();
}

void
burst::Display::Render( std::function<void(vk::CommandBuffer const &)> inComputeCallback, std::function<void(vk::CommandBuffer const &)> inPresentCallback  )
{
    std::uint32_t theFrameIndex = mSwapchain.RetrieveNextImage();

    // Begin the current frame's draw commands
    auto commandBuffer = mCommandBuffers[ theFrameIndex ];
    commandBuffer.begin( vk::CommandBufferBeginInfo( vk::CommandBufferUsageFlags() ) );
    {
        auto image = mSwapchain.GetImages()[ theFrameIndex ];

        /*
         * Process compute commands
         */
        inComputeCallback( commandBuffer );

        mRenderPass->Begin( commandBuffer, mFramebuffers[ theFrameIndex ]->GetVkFramebuffer(), vk::Rect2D( vk::Offset2D( 0, 0 ), mSwapchain.GetExtent() ), mClearValue );
        {
            // Set dynamic state
            commandBuffer.setViewport( 0, vk::Viewport( 0.0f, 0.0f, mSwapchain.GetExtent().width, mSwapchain.GetExtent().height, 0.0f, 1.0f ) );
            commandBuffer.setScissor( 0, vk::Rect2D( vk::Offset2D( 0, 0 ), mSwapchain.GetExtent() ) );

            // Draw screen rect
            mPipeline->Bind( commandBuffer );
            commandBuffer.draw( 3, 1, 0, 0 );

            /*
             * Process present commands
             * TODO: How should the interface to Display look?
             */
            inPresentCallback( commandBuffer );

        }
        commandBuffer.endRenderPass();
    }
    commandBuffer.end();

    mSwapchain.SubmitCommandBuffer( theFrameIndex, commandBuffer );
}

void
burst::Display::InitializeCommandBuffers()
{
    auto theCommandBufferCount = mSwapchain.GetImageCount();
    mCommandBuffers.resize( theCommandBufferCount );
    for( int i = 0; i < theCommandBufferCount; i++ )
    {
        mCommandBuffers[ i ] = mDevice.GetVkDevice().allocateCommandBuffers
        (
            vk::CommandBufferAllocateInfo( mDevice.GetVkCommandPool(), vk::CommandBufferLevel::ePrimary, 1 )
        ).front();
    }
}

void
burst::Display::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain.GetImageViews();
    mFramebuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFramebuffers.size(); i++)
    {
        std::vector< vk::ImageView > theAttachments =
        {
            theSwapchainImageViews[ i ]
        };
        mFramebuffers[i] = std::make_shared< vkt::FrameBuffer >
        (
            mDevice,
            mRenderPass,
            theAttachments,
            mSwapchain.GetExtent().width,
            mSwapchain.GetExtent().height
        );
    }
}

void
burst::Display::InitializePipeline( vk::RenderPass inRenderPass )
{
    auto vertexShader = vkt::Shader::CreateVkShaderModule( mDevice, "resources/shaders/ScreenRect.vert" );
    auto fragmentShader = vkt::Shader::CreateVkShaderModule( mDevice, "resources/shaders/Sampler.frag" );

    mPipeline = vkt::GraphicsPipelineBuilder( mDevice )
        .SetDescriptorSetLayouts( mDescriptorSetLayout )
        .SetVertexShader( vertexShader )
        .SetFragmentShader( fragmentShader )
        .SetRenderPass( inRenderPass )
        .Build();

    mDevice.GetVkDevice().destroy( vertexShader );
    mDevice.GetVkDevice().destroy( fragmentShader );
}