#include <iostream>

#include "burst/Engine.h"

class ExampleEngine
:
    public burst::Engine
{
    public:
        ExampleEngine( std::size_t inWidth, std::size_t inHeight, const char * inTitle )
        :
            burst::Engine( inWidth, inHeight, inTitle )
        {
        }

        virtual void Update() const override
        {
        }

        virtual void Render( vk::CommandBuffer inCommandBuffer ) const override
        {
        }

};

int main()
{
    auto engine = ExampleEngine( 800, 600, "Burst example" );
    engine.Run();
}