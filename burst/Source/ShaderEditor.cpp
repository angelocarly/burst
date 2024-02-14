//
// Created by magnias on 1/28/24.
//

#include "burst/ShaderEditor.h"

#include "vkt/Device.h"
#include "vkt/ComputePipeline.h"
#include "vkt/Shader.h"
#include "vkt/DescriptorSetLayout.h"

#include <glm/vec2.hpp>

#include <iostream>
#include <fstream>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <string>
#include <array>
#include <spdlog/spdlog.h>

burst::ShaderEditor::ShaderEditor( vkt::Device const & inDevice )
:
    mDevice( inDevice ),
    mPath( "test.comp.glsl" )
{
    Init( mPath );
}

burst::ShaderEditor::~ShaderEditor()
{

}

void
burst::ShaderEditor::Init( std::string inPath )
{
    // Initialize the editor and watch for changes
    glm::vec2 pixelScale = glm::vec2( 1.0f );
    mZep = std::make_shared<ZepWrapper>( inPath, Zep::NVec2f(pixelScale.x, pixelScale.y), [](std::shared_ptr<Zep::ZepMessage> spMessage) -> void {
    });

    // This is an example of adding different fonts for text styles.
    // If you ":e test.md" in the editor and type "# Heading 1" you will
    // see that Zep picks a different font size for the heading.
    auto& display = mZep->GetEditor().GetDisplay();
    auto pImFont = ImGui::GetIO().Fonts[0].Fonts[0];
    auto pixelHeight = pImFont->FontSize;
    display.SetFont(Zep::ZepTextType::UI, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight)));
    display.SetFont(Zep::ZepTextType::Text, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight)));
    display.SetFont(Zep::ZepTextType::Heading1, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight * 1.5)));
    display.SetFont(Zep::ZepTextType::Heading2, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight * 1.25)));
    display.SetFont(Zep::ZepTextType::Heading3, std::make_shared<Zep::ZepFont_ImGui>(display, pImFont, int(pixelHeight * 1.125)));

    mZep->GetEditor().InitWithFileOrDir( inPath );
}

void
burst::ShaderEditor::Update()
{
    if( mZep )
    {
        mZep->GetEditor().RefreshRequired();
    }
}

void
burst::ShaderEditor::Display()
{
    bool show = true;
    auto displaySize = glm::vec2( 10.0f, 10.0f );
    ImGui::SetNextWindowSize(ImVec2(displaySize.x, displaySize.y), ImGuiCond_FirstUseEver);
    if (!ImGui::Begin("Zep", &show, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_MenuBar))
    {
        ImGui::End();
        return;
    }

    if( ImGui::Button( "Save and compile" ) )
    {
        Save();
        CompileShader();
    }

    auto min = ImGui::GetCursorScreenPos();
    auto max = ImGui::GetContentRegionAvail();
    if (max.x <= 0)
        max.x = 1;
    if (max.y <= 0)
        max.y = 1;
    ImGui::InvisibleButton("ZepContainer", max);

    // Fill the window
    max.x = min.x + max.x;
    max.y = min.y + max.y;

    mZep->zepEditor.SetDisplayRegion(Zep::NVec2f(min.x, min.y), Zep::NVec2f(max.x, max.y));
    mZep->zepEditor.Display();
    bool zep_focused = ImGui::IsWindowFocused();
    if (zep_focused)
    {
        mZep->zepEditor.HandleInput();
    }

    // TODO: A Better solution for this; I think the audio graph is creating a new window and stealing focus
    static int focus_count = 0;
    if (focus_count++ < 2)
    {
        ImGui::SetWindowFocus();
    }
    ImGui::End();
}

void
burst::ShaderEditor::BindShader()
{

}

void
burst::ShaderEditor::Save()
{
    int64_t size;
    mZep->GetEditor().GetEditor().GetActiveBuffer()->Save( size );
}

void
burst::ShaderEditor::CompileShader()
{
    // First compile using glslc
    std::stringstream command;
    command << "/usr/bin/glslangValidator --target-env vulkan1.0 -V " << mPath << " -o " << mPath << ".spv";

    spdlog::info( "Running command: {}", command.str() );

    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr< FILE, decltype(&pclose) > pipe( popen( command.str().c_str(), "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), buffer.size(), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    spdlog::info( "Compiled shader output: {}", result );

    // Build shader
    auto shaderModule = vkt::Shader::CreateVkShaderModule( mDevice, mPath );

    if( !mLayouts )
    {
        mLayouts = vkt::DescriptorSetLayoutBuilder( mDevice )
            .AddLayoutBinding( 0, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eCompute )
            .AddLayoutBinding( 1, vk::DescriptorType::eStorageImage, vk::ShaderStageFlagBits::eCompute )
            .Build( vk::DescriptorSetLayoutCreateFlagBits::ePushDescriptorKHR );
    }

    std::vector< vk::PushConstantRange > pushConstants =
    {
        vk::PushConstantRange
        (
            vk::ShaderStageFlagBits::eCompute,
            0,
            sizeof( PushConstants )
        )
    };

    mCompute = vkt::ComputePipelineBuilder( mDevice )
        .SetComputeShader( shaderModule )
        .SetDescriptorSetLayouts( mLayouts )
        .SetPushConstants( pushConstants )
        .Build();
}

vkt::ComputePipelinePtr
burst::ShaderEditor::GetShader() const
{
    return mCompute;
}
