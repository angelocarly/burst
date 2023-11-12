#include "vkt/DescriptorSetLayout.h"

// ========================================= DescriptorSetLayout =======================================================

vkt::DescriptorSetLayouts::DescriptorSetLayouts( const vkt::Device & inDevice, std::vector< vk::DescriptorSetLayout > inDescriptorSetLayouts )
:
    mDevice( inDevice ),
    mDescriptorSetLayouts( inDescriptorSetLayouts )
{
}

vkt::DescriptorSetLayouts::~DescriptorSetLayouts()
{
    for( auto const & descriptorSetLayout : mDescriptorSetLayouts )
    {
        mDevice.GetVkDevice().destroyDescriptorSetLayout( descriptorSetLayout );
    }
}

std::vector< vk::DescriptorSetLayout > const &
vkt::DescriptorSetLayouts::GetVkDescriptorSetLayouts() const
{
    return mDescriptorSetLayouts;
}

// ========================================= DescriptorSetLayoutBuilder ================================================

vkt::DescriptorSetLayoutBuilder::DescriptorSetLayoutBuilder( vkt::Device const & inDevice )
:
    mDevice( inDevice )
{
}

vkt::DescriptorSetLayoutBuilder::~DescriptorSetLayoutBuilder()
{
}

vkt::DescriptorSetLayoutBuilder &
vkt::DescriptorSetLayoutBuilder::AddLayoutBinding
(
    std::uint32_t inBinding,
    vk::DescriptorType inDescriptorType,
    vk::ShaderStageFlags inShaderStageFlags
)
{
    mLayoutBindings.emplace_back( inBinding, inDescriptorType, 1, inShaderStageFlags );
    return * this;
}

vkt::DescriptorSetLayoutsPtr
vkt::DescriptorSetLayoutBuilder::Build( vk::DescriptorSetLayoutCreateFlags inFlags )
{
    auto theDescriptorSetLayoutCreateInfo = vk::DescriptorSetLayoutCreateInfo
    (
        inFlags,
        mLayoutBindings.size(),
        mLayoutBindings.data()
    );

    std::vector< vk::DescriptorSetLayout > theDescriptorSetLayouts;
    theDescriptorSetLayouts.push_back
    (
        mDevice.GetVkDevice().createDescriptorSetLayout
        (
            theDescriptorSetLayoutCreateInfo
        )
    );

    return std::make_shared< vkt::DescriptorSetLayouts >( mDevice, theDescriptorSetLayouts );
}
