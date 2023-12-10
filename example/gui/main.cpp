#include <iostream>

#include "burst/Engine.h"

#include "example/gui/TestPresenter.h"

#include <ImGuiFileDialog.h>

class ExampleEngine
:
    public burst::Engine
{
    public:
        ExampleEngine( std::size_t inWidth, std::size_t inHeight, const char * inTitle )
        :
            burst::Engine( inWidth, inHeight, inTitle ),
            mPresenter( GetPresentContext() )
        {
        }

        ~ExampleEngine()
        {
            GetPresentContext().mDevice.GetVkDevice().waitIdle();
        }

        virtual void Update( float inDelta ) override
        {
            mPresenter.Update( inDelta );

            ImGui::Begin( "test" );
            if (ImGui::Button("Open File Dialog"))
                ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", ".cpp,.h,.hpp", ".");

            // display
            if (ImGuiFileDialog::Instance()->Display("ChooseFileDlgKey"))
            {
                // action if OK
                if (ImGuiFileDialog::Instance()->IsOk())
                {
                    std::string filePathName = ImGuiFileDialog::Instance()->GetFilePathName();
                    std::string filePath = ImGuiFileDialog::Instance()->GetCurrentPath();
                    // action
                }

                // close
                ImGuiFileDialog::Instance()->Close();
            }
            ImGui::End();
        }

        virtual burst::Presenter & GetPresenter() const override
        {
            return ( burst::Presenter & ) mPresenter;
        }

    private:
        example::gui::TestPresenter mPresenter;
};

int main()
{
    auto engine = ExampleEngine( 1600, 900, "Gui example" );
    engine.Run();
}