#ifndef Vkt_DescriptorSetLayout_h
#define Vkt_DescriptorSetLayout_h

#include "Device.h"

#include <vulkan/vulkan.hpp>

#include <vector>

namespace vkt
{
    class DescriptorSetLayouts
    {
        public:
            DescriptorSetLayouts( vkt::Device const & inDevice, std::vector< vk::DescriptorSetLayout > inDescriptorSetLayouts );
            ~DescriptorSetLayouts();

            std::vector< vk::DescriptorSetLayout > const & GetVkDescriptorSetLayouts() const;

        private:
            vkt::Device const & mDevice;
            std::vector< vk::DescriptorSetLayout > mDescriptorSetLayouts;

        friend class DescriptorSetLayoutBuilder;
    };

    class DescriptorSetLayoutBuilder
    {
        public:
            DescriptorSetLayoutBuilder( vkt::Device const & inDevice );
            ~DescriptorSetLayoutBuilder();

        public:
            vkt::DescriptorSetLayoutBuilder & AddLayoutBinding( std::uint32_t inBinding, vk::DescriptorType inDescriptorType, vk::ShaderStageFlags inShaderStageFlags );
            vkt::DescriptorSetLayoutsPtr Build( vk::DescriptorSetLayoutCreateFlags inFlags = vk::DescriptorSetLayoutCreateFlags() );

        private:
            vkt::Device const & mDevice;
            std::vector< vk::DescriptorSetLayoutBinding > mLayoutBindings;
    };
}

#endif
