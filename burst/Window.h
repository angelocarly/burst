#ifndef Burst_Window_h
#define Burst_Window_h

#include "vkt/Instance.h"

#include <vulkan/vulkan.hpp>
#include <GLFW/glfw3.h>
#include <memory>

namespace burst
{
    class Window
    {
        public:
            /**
             * Contruct a system window.
             * @param width Initial width
             * @param height Initial height
             */
            Window( vkt::Instance & inInstance, int inWidth, int inHeight, const char * inTitle );
            ~Window();

        public:
            GLFWwindow * GetGLFWWindow() const;

            /**
             * @brief Returns true if the window should close. I.e. the user has clicked the close button.
             */
            bool ShouldClose() const;

            /**
             * @brief Polls for input and resize events.
             */
            void Poll() const;

            vk::SurfaceKHR GetSurface() const;

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif
