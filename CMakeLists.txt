cmake_minimum_required(VERSION 3.3)
project(Burst VERSION 0.1.0)

set( PROJECT_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}" )

set( LIB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/lib" )

include_directories( "lib/VulkanMemoryAllocator-Hpp/include")
include_directories( "lib/VulkanMemoryAllocator/include")

add_subdirectory( vkt )
add_subdirectory( burst )
add_subdirectory( example )