#include <iostream>

#include "burst/Engine.h"

#include "TestPresenter.h"

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
        }

        virtual burst::Presenter & GetPresenter() const override
        {
            return ( burst::Presenter & ) mPresenter;
        }

    private:
        example::TestPresenter mPresenter;
};

int main()
{
    auto engine = ExampleEngine( 800, 600, "Burst example" );
    engine.Run();
}