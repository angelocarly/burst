#ifndef Burst_Window_h
#define Burst_Window_h

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
            Window( int width, int height, const char * title);
            ~Window();

        public:
            GLFWwindow * GetGLFWWindow();

            /**
             * @brief Returns true if the window should close. I.e. the user has clicked the close button.
             */
            bool ShouldClose() const;

            /**
             * @brief Polls for input and resize events.
             */
            void Poll() const;

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif
