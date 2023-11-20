#include "vkt/Mesh.h"

vkt::Vertex::Vertex( glm::vec3 inPosition, glm::vec3 inNormal, glm::vec3 inColor )
:
    mPosition( inPosition ),
    mNormal( inNormal ),
    mColor( inColor )
{

}

vkt::Vertex::~Vertex()
{

}

std::vector< vk::VertexInputAttributeDescription >
vkt::Vertex::GetVkVertexInputAttributeDescriptions()
{
    std::vector< vk::VertexInputAttributeDescription > theAttributes;
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            0,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, mPosition )
        )
    );
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            1,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, mNormal )
        )
    );
    theAttributes.push_back
    (
        vk::VertexInputAttributeDescription
        (
            2,
            0,
            vk::Format::eR32G32B32Sfloat,
            offsetof( Vertex, mColor )
        )
    );
    return theAttributes;
}

std::vector< vk::VertexInputBindingDescription >
vkt::Vertex::GetVkVertexInputBindingDescriptions()
{
    std::vector< vk::VertexInputBindingDescription > theDescriptions;
    theDescriptions.push_back
        (
            vk::VertexInputBindingDescription
                (
                    0,
                    sizeof( Vertex ),
                    vk::VertexInputRate::eVertex
                )
        );
    return theDescriptions;
}

// =====================================================================================================================

vkt::Mesh::Mesh( const vkt::Device & inDevice, std::vector< vkt::Vertex > inVertices, std::vector< std::uint32_t > inIndices )
:
    mDevice( inDevice )
{
    mVertexCount = inVertices.size();
    mIndexCount = inIndices.size();

    mVertexBuffer = vkt::BufferFactory( mDevice )
        .CreateBuffer
        (
            sizeof( vkt::Vertex ) * inVertices.size(),
            vk::BufferUsageFlagBits::eVertexBuffer,
            vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
            "Vertex Buffer"
        );

    void * theVertexData = mVertexBuffer->MapMemory();
    std::memcpy( theVertexData, inVertices.data(), sizeof( vkt::Vertex ) * inVertices.size() );
    mVertexBuffer->UnMapMemory();

    mIndexBuffer = vkt::BufferFactory( mDevice )
        .CreateBuffer
        (
            sizeof( std::uint32_t ) * inIndices.size(),
            vk::BufferUsageFlagBits::eIndexBuffer,
            vma::AllocationCreateFlagBits::eHostAccessSequentialWrite,
            "Index buffer"
        );

    void * theIndexData = mIndexBuffer->MapMemory();
    std::memcpy( theIndexData, inIndices.data(), sizeof( std::uint32_t ) * inIndices.size() );
    mIndexBuffer->UnMapMemory();
}

vkt::Mesh::~Mesh()
{

}

vkt::BufferPtr
vkt::Mesh::GetVertexBuffer()
{
    return mVertexBuffer;
}

vkt::BufferPtr
vkt::Mesh::GetIndexBuffer()
{
    return mIndexBuffer;
}

std::uint32_t
vkt::Mesh::GetVertexCount()
{
    return mVertexCount;
}

std::uint32_t
vkt::Mesh::GetIndexCount()
{
    return mIndexCount;
}

void
vkt::Mesh::Draw( vk::CommandBuffer inCommandBuffer )
{
    auto bufferData = mVertexBuffer->GetVkBuffer();
    auto offsets = vk::DeviceSize( 0 );
    inCommandBuffer.bindVertexBuffers( 0, 1, &bufferData, & offsets );
    inCommandBuffer.bindIndexBuffer( mIndexBuffer->GetVkBuffer(), 0, vk::IndexType::eUint32 );
    inCommandBuffer.drawIndexed( mIndexCount, 1, 0, 0, 0 );
}

