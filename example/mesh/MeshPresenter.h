#ifndef Example_MeshPresenter_h
#define Example_MeshPresenter_h

#include "burst/Presenter.h"

#include <glm/glm.hpp>

namespace example
{
    class MeshPresenter
    :
        public burst::Presenter
    {
        public:
            explicit MeshPresenter( burst::PresentContext const & inContext, std::function< void( vk::CommandBuffer ) > inMeshCallback );
            ~MeshPresenter();

            void Compute( vk::CommandBuffer inCommandBuffer ) const override;
            void Present( vk::CommandBuffer inCommandBuffer ) const override;

        private:
            std::function< void( vk::CommandBuffer ) > mMeshCallback;

            burst::PresentContext const & mContext;
            vkt::MeshPtr mMesh;

            vkt::DescriptorSetLayoutsPtr mGraphicsDescriptorSetLayout;
            vkt::GraphicsPipelinePtr mPipeline;
    };
}

#endif
