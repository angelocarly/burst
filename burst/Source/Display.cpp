#include "burst/Display.h"

#include "vkt/GraphicsPipeline.h"
#include "vkt/Shader.h"
#include "vkt/DescriptorSetLayout.h"

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
    InitializeRenderPass();
    InitializeFrameBuffers();
    InitializePipeline( mRenderPass );
}

burst::Display::~Display()
{
    mDevice.GetVkDevice().waitIdle();
    mDevice.GetVkDevice().destroy( mRenderPass );
    for( auto framebuffer : mFramebuffers )
    {
        mDevice.GetVkDevice().destroy( framebuffer );
    }
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

        commandBuffer.beginRenderPass( CreateRenderPassBeginInfo( theFrameIndex ), vk::SubpassContents::eInline );
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
burst::Display::InitializeRenderPass()
{
    auto theColorAttachmentReference = vk::AttachmentReference
    (
        0,
        vk::ImageLayout::eColorAttachmentOptimal
    );

    auto theSubpassDescription = vk::SubpassDescription
    (
        vk::SubpassDescriptionFlags(),
        vk::PipelineBindPoint::eGraphics,
        0,
        nullptr,
        1,
        & theColorAttachmentReference,
        nullptr,
        nullptr,
        0,
        nullptr
    );

    // The depth attachment is first accessed in the early fragment stage.
    std::array< vk::SubpassDependency, 1 > theSubPassDependencies;
    theSubPassDependencies[ 0 ].setSrcSubpass( VK_SUBPASS_EXTERNAL );
    theSubPassDependencies[ 0 ].setDstSubpass( 0 );
    theSubPassDependencies[ 0 ].setSrcStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
    theSubPassDependencies[ 0 ].setSrcAccessMask( vk::AccessFlagBits::eNone );
    theSubPassDependencies[ 0 ].setDstStageMask( vk::PipelineStageFlagBits::eColorAttachmentOutput | vk::PipelineStageFlagBits::eEarlyFragmentTests );
    theSubPassDependencies[ 0 ].setDstAccessMask( vk::AccessFlagBits::eColorAttachmentWrite | vk::AccessFlagBits::eDepthStencilAttachmentWrite );

    auto theColorAttachmentDescription = vk::AttachmentDescription
    (
        vk::AttachmentDescriptionFlags(),
        mSwapchain.GetImageFormat(),
        vk::SampleCountFlagBits::e1,
        vk::AttachmentLoadOp::eClear,
        vk::AttachmentStoreOp::eStore,
        vk::AttachmentLoadOp::eDontCare,
        vk::AttachmentStoreOp::eDontCare,
        vk::ImageLayout::eUndefined,
        vk::ImageLayout::ePresentSrcKHR
    );

    std::vector< vk::AttachmentDescription > theAttachmentDescriptions =
    {
        theColorAttachmentDescription
    };
    auto theRenderPassCreateInfo = vk::RenderPassCreateInfo
    (
        vk::RenderPassCreateFlags(),
        theAttachmentDescriptions.size(),
        theAttachmentDescriptions.data(),
        1,
        &theSubpassDescription,
        theSubPassDependencies.size(),
        theSubPassDependencies.data()
    );

    mRenderPass = mDevice.GetVkDevice().createRenderPass
    (
        theRenderPassCreateInfo
    );

    mClearColor = vk::ClearColorValue( std::array< float, 4 >{ 0.0f, 0.0f, 0.0f, 1.0f } );
    mClearValue = vk::ClearValue( mClearColor );
}

void
burst::Display::InitializeFrameBuffers()
{
    auto theSwapchainImageViews = mSwapchain.GetImageViews();
    mFramebuffers.resize( theSwapchainImageViews.size() );

    for ( size_t i = 0; i < mFramebuffers.size(); i++)
    {
        std::array< vk::ImageView, 1 > theAttachments =
        {
            theSwapchainImageViews[ i ]
        };
        auto theFrameBufferCreateInfo = vk::FramebufferCreateInfo
        (
            vk::FramebufferCreateFlags(),
            mRenderPass,
            theAttachments.size(),
            theAttachments.data(),
            mSwapchain.GetExtent().width,
            mSwapchain.GetExtent().height,
            1
        );
        mFramebuffers[i] = mDevice.GetVkDevice().createFramebuffer( theFrameBufferCreateInfo );
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

vk::RenderPassBeginInfo
burst::Display::CreateRenderPassBeginInfo( std::size_t inFrameIndex )
{
    auto theRenderPassBeginInfo = vk::RenderPassBeginInfo
    (
        mRenderPass,
        mFramebuffers[ inFrameIndex ],
        vk::Rect2D( vk::Offset2D( 0, 0 ), mSwapchain.GetExtent() ),
        1,
        &mClearValue
    );
    return theRenderPassBeginInfo;
}
