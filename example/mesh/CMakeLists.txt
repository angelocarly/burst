set(
    HEADER_FILES
        MeshPresenter.h
)

set(
    SOURCE_FILES
        Source/MeshPresenter.cpp
        Source/main.cpp
)

set(
    GLSL_SOURCE_FILES
        shaders/Mesh.vert
        shaders/Mesh.frag
)

add_executable(
    MeshExample
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET MeshExample PROPERTY CXX_STANDARD 17 )

## Project libraries
target_link_libraries(
    MeshExample PUBLIC
        Burst
)

include_directories( ${BURST_ROOT_DIR} )

compile_shaders( MeshExample "${CMAKE_CURRENT_SOURCE_DIR}/resources/" "${CMAKE_CURRENT_BINARY_DIR}/resources/shaders/" "${GLSL_SOURCE_FILES}" )

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( MeshExample PUBLIC spdlog::spdlog )
