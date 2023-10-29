#include "burst/Engine.h"

#include <spdlog/spdlog.h>
#include <spdlog/sinks/stdout_color_sinks.h>

burst::Engine::Engine()
{
    spdlog::set_level(spdlog::level::debug);
    spdlog::stdout_color_mt("burst");

    spdlog::get( "burst" )->info( "Started engine" );
}

burst::Engine::~Engine()
{
    spdlog::get( "burst" )->info( "Stopped engine" );
}
