#include "burst/GuiPresenter.h"

#include "burst/Utils.h"
#include "burst/Window.h"

#include "vkt/Swapchain.h"
#include "vkt/RenderPass.h"

#include <imgui.h>
#include <imgui_impl_vulkan.h>
#include <imgui_impl_glfw.h>

#include <spdlog/spdlog.h>

#include <filesystem>

namespace
{
    void StartImGuiFrame()
    {
        // Start the next Dear ImGui frame
        ImGui_ImplVulkan_NewFrame();
        ImGui_ImplGlfw_NewFrame();


        ImGuiIO& io = ImGui::GetIO();
        io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
        io.ConfigDockingWithShift = true;

        ImGui::NewFrame();

        // Allow docking with the main window
        ImGui::SetNextWindowBgAlpha( 0.0f );
        ImGui::DockSpaceOverViewport( nullptr, ImGuiDockNodeFlags_PassthruCentralNode );
    }
}


burst::GuiPresenter::GuiPresenter( const vkt::Device & inDevice, const burst::Window & inWindow, const vkt::Swapchain & inSwapchain, const vkt::RenderPass & inRenderPass )
:
    mDevice( inDevice )
{
    InitializeDescriptorPool();
    InitializeImGui( inWindow, inSwapchain, inRenderPass );
    StartImGuiFrame();
}

burst::GuiPresenter::~GuiPresenter()
{
    mDevice.WaitIdle();

    ImGui_ImplVulkan_Shutdown();
    ImGui_ImplGlfw_Shutdown();

    mDevice.GetVkDevice().destroy( mDescriptorPool );

    ::ImGui::DestroyContext();
}

void
burst::GuiPresenter::CheckVkResult( VkResult inResult )
{
    if ( inResult == 0 ) return;

    spdlog::error( "[ImGui][Vulkan] Error: VkResult = {}", std::to_string( inResult ) );

    if ( inResult < 0) abort();
}

void
burst::GuiPresenter::InitializeImGui( const burst::Window & inWindow, const vkt::Swapchain & inSwapchain, const vkt::RenderPass & inRenderPass )
{
    ::IMGUI_CHECKVERSION();
    ::ImGui::CreateContext();
    ImGuiIO & io = ::ImGui::GetIO();
    ( void ) io;

    SetImGuiStyle( io );

    // Load fonts
    // TODO: Load fonts dynamically
//    auto fontPath = std::filesystem::path( "./" ) / "resources" / "fonts" / "Hack-Regular.ttf";
//    io.Fonts->AddFontDefault();
//    io.Fonts->AddFontFromFileTTF( fontPath.string().c_str(), 16.0f );

    // Setup Vulkan bindings
    ImGui_ImplGlfw_InitForVulkan( inWindow.GetGLFWWindow(), true );
    ImGui_ImplVulkan_InitInfo init_info = {};
    init_info.Instance = mDevice.GetInstance().GetVkInstance();
    init_info.PhysicalDevice = mDevice.GetPhysicalDevice().GetVkPhysicalDevice();
    init_info.Device = mDevice.GetVkDevice();
    init_info.QueueFamily = mDevice.GetPhysicalDevice().FindQueueFamilyIndices().graphicsFamilyIndex.value();
    init_info.Queue = mDevice.GetQueue();
    init_info.PipelineCache = nullptr;
    init_info.DescriptorPool = mDescriptorPool;
    init_info.Subpass = 0;
    init_info.MinImageCount = inSwapchain.GetMinImageCount();
    init_info.ImageCount = inSwapchain.GetImageCount();
    init_info.MSAASamples = VK_SAMPLE_COUNT_1_BIT;
    init_info.Allocator = nullptr;
    init_info.CheckVkResultFn = GuiPresenter::CheckVkResult;
    ImGui_ImplVulkan_Init( & init_info, inRenderPass.GetVkRenderPass() );

    // Create fonts
    ImGui_ImplVulkan_CreateFontsTexture();
//    ImGui_ImplVulkan_DestroyFontsTexture();
}

void
burst::GuiPresenter::InitializeDescriptorPool()
{
    std::vector< vk::DescriptorPoolSize > thePoolSizes =
        {
            { vk::DescriptorType::eSampler, 1000 },
            { vk::DescriptorType::eCombinedImageSampler, 1000 },
            { vk::DescriptorType::eSampledImage, 1000 },
            { vk::DescriptorType::eStorageImage, 1000 },
            { vk::DescriptorType::eUniformTexelBuffer, 1000 },
            { vk::DescriptorType::eStorageTexelBuffer, 1000 },
            { vk::DescriptorType::eUniformBuffer, 1000 },
            { vk::DescriptorType::eStorageBuffer, 1000 },
            { vk::DescriptorType::eUniformBufferDynamic, 1000 },
            { vk::DescriptorType::eStorageBufferDynamic, 1000 },
            { vk::DescriptorType::eInputAttachment, 1000 }
        };

    vk::DescriptorPoolCreateInfo const theDescriptorPoolCreateInfo
    (
        vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet,
        1000, // Correct?
        thePoolSizes.size(),
        thePoolSizes.data()
    );

    mDescriptorPool = mDevice.GetVkDevice().createDescriptorPool( theDescriptorPoolCreateInfo );
}

void
burst::GuiPresenter::SetImGuiStyle( ImGuiIO & io )
{
    ImGui::StyleColorsDark();
    ImGuiStyle* theStyle = &ImGui::GetStyle();
    theStyle->WindowBorderSize = 0.0f;
    theStyle->WindowPadding = ImVec2( 4, 4 );
    theStyle->FramePadding = ImVec2( 4, 2 );
    theStyle->ItemSpacing = ImVec2( 8, 4 );

    // Double the scale on Mac displays as ImGui doesn't provide appropriate font sizes for them.
    // https://github.com/ocornut/imgui/issues/5301
    if( burst::Utils::IsTargetApple() )
    {
        float scale = 2.0f;
        ImFontConfig cfg;
        cfg.SizePixels = 13.0f * scale;
        io.Fonts->AddFontDefault( & cfg );
        io.FontGlobalScale = 1.0f / scale;
    }

    ImVec4 theBaseColor = ImVec4( 0.71f, .51f, .31f, 1.0f );
    ImVec4 theHoverColor = ImVec4( .75f, .48f, .22f, 1.0f );
    ImVec4 theActiveColor = ImVec4( .61f, .36f, .10f, 1.0f );

    ImVec4 theSecondColor = ImVec4( 0.32f, .31f, .29f, 1.0f );
    ImVec4 theSecondHoverColor = ImVec4( .44f, .44f, .40f, 1.0f );
    ImVec4 theSecondActiveColor = ImVec4( .43f, .36f, .29f, 1.0f );

    ImVec4 theThirdColor = ImVec4( 0.18f, .17f, .16f, 1.0f );
    ImVec4 theThirdHoverColor = ImVec4( .40f, .33f, .25f, 1.0f );
    ImVec4 theThirdActiveColor = ImVec4( .43f, .36f, .29f, 1.0f );

    ImVec4* colors = theStyle->Colors;
    colors[ ImGuiCol_DockingEmptyBg ] = ImVec4( 0.0f, 0.0f, 0.0f, 0.0f );

    colors[ ImGuiCol_TabUnfocusedActive ] = ImVec4( 0.68f, 0.47f, 0.24f, 1.0f );
    colors[ ImGuiCol_SeparatorHovered ] = ImVec4( .54f, .40f, .08f, 0.75f );
    colors[ ImGuiCol_SeparatorActive ] = ImVec4( .75f, .51f, .1f, 1.0f );

    colors[ ImGuiCol_Button ] = theBaseColor;
    colors[ ImGuiCol_ButtonHovered ] = theHoverColor;
    colors[ ImGuiCol_ButtonActive ] = theActiveColor;
    colors[ ImGuiCol_Header ] = theThirdColor;
    colors[ ImGuiCol_HeaderHovered ] = theThirdHoverColor;
    colors[ ImGuiCol_HeaderActive ] = theThirdActiveColor;
    colors[ ImGuiCol_Tab ] = theBaseColor;
    colors[ ImGuiCol_TabHovered ] = theHoverColor;
    colors[ ImGuiCol_TabActive ] = theActiveColor;
    colors[ ImGuiCol_TitleBgActive ] = theBaseColor;
    colors[ ImGuiCol_ResizeGrip ] = theBaseColor;
    colors[ ImGuiCol_ResizeGripHovered ] = theHoverColor;
    colors[ ImGuiCol_ResizeGripActive ] = theActiveColor;
    colors[ ImGuiCol_SliderGrab ] = theBaseColor;
    colors[ ImGuiCol_SliderGrabActive ] = theActiveColor;
    colors[ ImGuiCol_FrameBg ] = theSecondColor;
    colors[ ImGuiCol_FrameBgHovered ] = theSecondHoverColor;
    colors[ ImGuiCol_FrameBgActive ] = theSecondActiveColor;
    colors[ ImGuiCol_CheckMark ] = theBaseColor;
}

void
burst::GuiPresenter::Compute( vk::CommandBuffer inCommandBuffer ) const
{

}

void
burst::GuiPresenter::Present( vk::CommandBuffer inCommandBuffer ) const
{
    // Draw to screen
    ImGui::Render();

    ImGui_ImplVulkan_RenderDrawData( ImGui::GetDrawData(), inCommandBuffer );

    StartImGuiFrame();
}
