#ifndef Vkt_Mesh_h
#define Vkt_Mesh_h

#include "vkt/Device.h"
#include "vkt/Device.h"
#include "Buffer.h"

#include <glm/glm.hpp>

#include <vector>

namespace vkt
{
    class Vertex
    {
        public:
            Vertex( glm::vec3 inPosition, glm::vec3 inNormal, glm::vec3 inColor );
            ~Vertex();

        public:
            static std::vector< vk::VertexInputAttributeDescription > GetVkVertexInputAttributeDescriptions();
            static std::vector< vk::VertexInputBindingDescription > GetVkVertexInputBindingDescriptions();

        private:
            glm::vec3 mPosition;
            glm::vec3 mNormal;
            glm::vec3 mColor;
    };

    class Mesh
    {
        public:
            Mesh( vkt::Device const & inDevice, std::vector< vkt::Vertex > inVertices, std::vector< std::uint32_t > inIndices );
            ~Mesh();

            vkt::BufferPtr GetVertexBuffer();
            vkt::BufferPtr GetIndexBuffer();
            uint32_t GetVertexCount();
            uint32_t GetIndexCount();

            void Draw( vk::CommandBuffer inCommandBuffer );

        private:
            vkt::Device const & mDevice;

            std::uint32_t mVertexCount;
            std::uint32_t mIndexCount;
            vkt::BufferPtr mVertexBuffer;
            vkt::BufferPtr mIndexBuffer;
    };
}

#endif
