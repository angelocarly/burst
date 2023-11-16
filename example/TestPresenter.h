#ifndef Example_TestPresenter_h
#define Example_TestPresenter_h

#include "burst/Presenter.h"

#include "vkt/Device.h"
#include "vkt/ForwardDecl.h"
#include "vkt/Image.h"

#include <chrono>

namespace example
{
    class TestPresenter
    :
        public burst::Presenter
    {
        public:
            TestPresenter( burst::PresentContext const & inContext );
            ~TestPresenter();

            void Compute( vk::CommandBuffer inCommandBuffer ) const override;
            void Present( vk::CommandBuffer inCommandBuffer ) const override;

        private:
            burst::PresentContext const & mContext;

            vkt::ImagePtr mImage;
            vk::Sampler mSampler;
            vk::ImageView mImageView;

            vkt::DescriptorSetLayoutsPtr mComputeDescriptorSetLayout;
            vkt::ComputePipelinePtr mComputePipeline;

            vkt::DescriptorSetLayoutsPtr mGraphicsDescriptorSetLayout;
            vkt::GraphicsPipelinePtr mPipeline;

            std::chrono::microseconds mStartTime = std::chrono::duration_cast<std::chrono::microseconds>
            (
                std::chrono::system_clock::now().time_since_epoch()
            );

            struct PushConstants
            {
                float mTime;
            };
    };
}

#endif
