//
// Created by magnias on 1/28/24.
//

#ifndef FEEDBACKLOOP_SHADEREDITOR_H
#define FEEDBACKLOOP_SHADEREDITOR_H

#include "zep.h"

#include "vkt/Device.h"

namespace burst
{
    class ShaderEditor
    {
        public:
            ShaderEditor( vkt::Device const & inDevice );
            ~ShaderEditor();

            void Update();
            void Display();
            void BindShader();

            vkt::ComputePipelinePtr GetShader() const;

        private:

            struct PushConstants
            {
                float mTime;
                std::uint32_t mWidth;
                std::uint32_t mHeight;
                float mSlider0;
            };

            /**
             * Store the current buffer to a file
             */
            void Save();

            void CompileShader();

            void Init( std::string inPath );

            struct ZepWrapper : public Zep::IZepComponent
            {
                ZepWrapper(const std::filesystem::path& root_path, const Zep::NVec2f& pixelScale, std::function<void(std::shared_ptr<Zep::ZepMessage>)> fnCommandCB)
                    : zepEditor(std::filesystem::path(root_path.string()), pixelScale)
                    , Callback(fnCommandCB)
                {
                    zepEditor.RegisterCallback(this);

                }

                virtual Zep::ZepEditor& GetEditor() const override
                {
                    return (Zep::ZepEditor&)zepEditor;
                }

                virtual void Notify(std::shared_ptr<Zep::ZepMessage> message) override
                {
                    Callback(message);

                    return;
                }

                virtual void HandleInput()
                {
                    zepEditor.HandleInput();
                }

                Zep::ZepEditor_ImGui zepEditor;
                std::function<void(std::shared_ptr<Zep::ZepMessage>)> Callback;
            };

            std::shared_ptr< ZepWrapper > mZep;

            std::string const mPath;

            /// Shader data
            vkt::Device const & mDevice;

            // Let's just have a compute shader for now
            vkt::DescriptorSetLayoutsPtr mLayouts;
            vkt::ComputePipelinePtr mCompute;
    };
}

#endif //FEEDBACKLOOP_SHADEREDITOR_H
