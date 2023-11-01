#include "burst/Window.h"

#include <GLFW/glfw3.h>

#include <string>

// ============================================== Impl =================================================================

class burst::Window::Impl
{
    public:
        Impl( int width, int height, const char * title );
        ~Impl();

        void CreateWindow( int width, int height, const char * title );

        struct DestroyWindow
        {
            void operator()( GLFWwindow * ptr ) const
            {
                glfwDestroyWindow( ptr );
                glfwTerminate();
            }
        };

    public:
        std::unique_ptr< GLFWwindow, DestroyWindow > mWindow;
};

burst::Window::Impl::Impl(int inWidth, int inHeight, const char * inTitle )
{
    CreateWindow( inWidth, inHeight, inTitle );
}

burst::Window::Impl::~Impl() = default;

void
burst::Window::Impl::CreateWindow( int width, int height, const char * title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->mWindow.reset( glfwCreateWindow( width, height, title, nullptr, nullptr) );

}

// ============================================== Window ===============================================================

burst::Window::Window( int width, int height, const char * title )
:
    mImpl( new Impl( width, height, title ) )
{
}

burst::Window::~Window() = default;

bool
burst::Window::ShouldClose() const
{
    return glfwWindowShouldClose( mImpl->mWindow.get() );
}

void
burst::Window::Poll() const
{
    glfwPollEvents();
}

GLFWwindow *
burst::Window::GetGLFWWindow()
{
    return mImpl->mWindow.get();
}