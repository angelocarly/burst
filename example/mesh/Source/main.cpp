#include <iostream>

#include "burst/Engine.h"
#include "vkt/Mesh.h"

#include "example/mesh/MeshPresenter.h"

class ExampleEngine
:
    public burst::Engine
{
    public:
        void Draw( vk::CommandBuffer inCommandBuffer )
        {
            mMesh->Draw( inCommandBuffer );
        }

        ExampleEngine( std::size_t inWidth, std::size_t inHeight, const char * inTitle )
        :
            burst::Engine( inWidth, inHeight, inTitle ),
            mPresenter( GetPresentContext(), std::bind( & ExampleEngine::Draw, this, std::placeholders::_1 ) )
        {

            std::vector< vkt::Vertex > vertices =
            {
                vkt::Vertex( glm::vec3( -0.5f, 0.5f, 0.0f ), glm::vec3(), glm::vec3( 0.0f, 1.0f, 0.0f ) ),
                vkt::Vertex( glm::vec3( 0.0f, -0.5f, 0.0f ), glm::vec3(), glm::vec3( 0.0f, 0.0f, 1.0f ) ),
                vkt::Vertex( glm::vec3( 0.5f, 0.5f, 0.0f ), glm::vec3(), glm::vec3( 1.0f, 0.0f, 0.0f ) )
            };
            std::vector< std::uint32_t > indices =
            {
                0, 1, 2
            };
            mMesh = std::make_shared< vkt::Mesh >
            (
                GetPresentContext().mDevice,
                vertices,
                indices
            );
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
        example::MeshPresenter mPresenter;
        vkt::MeshPtr mMesh;
};

int main()
{
    auto engine = ExampleEngine( 800, 600, "Burst example" );
    engine.Run();
}