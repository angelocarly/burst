#include "burst/Window.h"
#include "vkt/Instance.h"

#include <vulkan/vulkan.hpp>

#include <GLFW/glfw3.h>

#include <glm/glm.hpp>

#include <string>
#include <spdlog/spdlog.h>

#include <filesystem>

// ============================================== Impl =================================================================

class burst::Window::Impl
{
    public:
        Impl( vkt::Instance & inInstance, int inWidth, int inHeight, const char * inTitle );
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
        vk::SurfaceKHR mSurface;
        vkt::Instance const & mInstance;

    private:
        vk::SurfaceKHR CreateSurface( vkt::Instance & inInstance );
};

burst::Window::Impl::Impl( vkt::Instance & inInstance, int inWidth, int inHeight, const char * inTitle )
:
    mInstance( inInstance )
{
    CreateWindow( inWidth, inHeight, inTitle );
    mSurface = CreateSurface( inInstance );
}

burst::Window::Impl::~Impl()
{
    spdlog::get( "vkt" )-> debug( "Destroying window" );

    mInstance.GetVkInstance().destroy( mSurface );
}

void
burst::Window::Impl::CreateWindow( int width, int height, const char * title)
{
    glfwInit();

    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

    this->mWindow.reset( glfwCreateWindow( width, height, title, nullptr, nullptr) );

}

vk::SurfaceKHR
burst::Window::Impl::CreateSurface( vkt::Instance & inInstance )
{
    VkSurfaceKHR theVkSurface;
    glfwCreateWindowSurface
    (
        inInstance.GetVkInstance(),
        mWindow.get(),
        nullptr,
        & theVkSurface
    );
    return theVkSurface;
}

// ============================================== Window ===============================================================

burst::Window::Window( vkt::Instance & inInstance, int inWidth, int inHeight, const char * inTitle )
:
    mImpl( new Impl( inInstance, inWidth, inHeight, inTitle ) )
{
}

burst::Window::~Window()
{
};

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
burst::Window::GetGLFWWindow() const
{
    return mImpl->mWindow.get();
}

vk::SurfaceKHR
burst::Window::GetSurface() const
{
    return mImpl->mSurface;
}

glm::ivec2
burst::Window::GetSize() const
{
    glm::ivec2 size;
    glfwGetWindowSize( mImpl->mWindow.get(), & size.x, & size.y );
    return size;
}
