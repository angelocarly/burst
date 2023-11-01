#ifndef Burst_Engine_h
#define Burst_Engine_h

#include "Window.h"
#include "vkt/Instance.h"

namespace burst
{
    class Engine
    {
        public:
            Engine( std::size_t inWidth, std::size_t inHeight, const char * inTitle );
            ~Engine();

        public:
            void Run() const;

        private:
            vkt::Instance CreateInstance() const;

            burst::Window mWindow;
            vkt::Instance mInstance;

    };
}

#endif
