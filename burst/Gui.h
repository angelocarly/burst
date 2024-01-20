#ifndef Burst_Gui
#define Burst_Gui

#include <glm/glm.hpp>

#include <vulkan/vulkan.hpp>

#include <string>
#include <functional>

namespace burst::gui
{
    class ImageInspector
    {
        public:
            ImageInspector( std::string inTitle, vk::Extent2D inExtent, std::vector< std::pair< vk::Sampler, vk::ImageView > > inImages, std::function< void( glm::vec2 ) > inOnDraw = []( glm::vec2 ){} );
            ~ImageInspector();

            void Update( std::vector< bool > inShowLayers );
            glm::vec2 GetCurrentPixel();

        private:
            std::string mTitle;
            vk::Extent2D mExtent;
            glm::vec2 mTranslate = glm::vec2( 0, 0 );
            glm::vec2 mScale = glm::vec2( 1, 1 );

            std::function< void( glm::vec2 inPos ) > mOnDraw;

            std::vector< VkDescriptorSet > mDescriptorSets;

            glm::vec2 mCurrentPixel;

    };
}

#endif
