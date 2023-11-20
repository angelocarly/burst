set(
    HEADER_FILES
        TestPresenter.h
)

set(
    SOURCE_FILES
        Source/TestPresenter.cpp
        main.cpp
)

set(
    GLSL_SOURCE_FILES
        shaders/Sampler.frag
        shaders/Compute.comp
)

add_executable(
    ComputeExample
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET ComputeExample PROPERTY CXX_STANDARD 17 )

## Project libraries
target_link_libraries(
    ComputeExample PUBLIC
        Burst
)

include_directories( ${BURST_ROOT_DIR} )


compile_shaders( ComputeExample "${CMAKE_CURRENT_SOURCE_DIR}/resources/" "${CMAKE_CURRENT_BINARY_DIR}/resources/shaders/" "${GLSL_SOURCE_FILES}" )
