#ifndef Burst_Window_h
#define Burst_Window_h

#include <GLFW/glfw3.h>
#include <memory>

namespace burst
{
    class Window
    {
        public:
            Window( int width, int height, const char * title);
            ~Window();

        public:
            GLFWwindow * GetGLFWWindow();

        public:
            bool ShouldClose() const;
            void Poll() const;

        private:
            class Impl;
            std::unique_ptr< Impl > mImpl;
    };
}

#endif
