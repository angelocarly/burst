#ifndef Vkt_Pipeline_h
#define Vkt_Pipeline_h

#include "vkt/Device.h"

#include <vulkan/vulkan.hpp>

namespace vkt
{
    class Pipeline
    {
        public:
            Pipeline( vkt::Device const & inDevice, vk::PipelineBindPoint inPipelineBindPoint, std::tuple< vk::Pipeline, vk::PipelineLayout > inPipeline );
            ~Pipeline();

            void Bind( vk::CommandBuffer inCommandBuffer );
            void BindPushDescriptorSet( vk::CommandBuffer inCommandBuffer, vk::WriteDescriptorSet inWriteDescriptorSet );

            vk::PipelineLayout GetVkPipelineLayout() const;

        private:
            vkt::Device const & mDevice;

            vk::PipelineBindPoint const mPipelineBindPoint;
            vk::Pipeline mPipeline;
            vk::PipelineLayout mPipelineLayout;
    };
}

#endif
