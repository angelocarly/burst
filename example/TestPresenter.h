#ifndef Example_TestPresenter_h
#define Example_TestPresenter_h

#include "burst/Presenter.h"

#include "vkt/Device.h"
#include "vkt/ForwardDecl.h"
#include "vkt/Image.h"

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
    };
}

#endif
