#include <iostream>

#include "burst/Engine.h"

#include "example/gui/TestPresenter.h"

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