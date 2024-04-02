cmake_minimum_required(VERSION 3.5)
project(Burst VERSION 0.1.0)

set(ENV{USE_WAYLAND_WSI} "ON")

include( Burst.cmake )

set( BURST_ROOT_DIR "${CMAKE_CURRENT_LIST_DIR}" )

add_subdirectory( resources )

add_subdirectory( lib )
add_subdirectory( vkt )
add_subdirectory( burst )
add_subdirectory( example )
