set(
    HEADER_FILES
        Engine.h
        Window.h
)

set(
    SOURCE_FILES
        Source/Engine.cpp
        Source/Window.cpp
)

add_library(
    Burst SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET ${LIBRARY_OUTPUT_NAME} PROPERTY CXX_STANDARD 17 )
include_directories( ${PROJECT_ROOT_DIR} )

# Project libraries
target_link_libraries(
    Burst PUBLIC
        Vkt
)

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Burst PUBLIC spdlog::spdlog )

# GLFW
find_package( glfw3 3.3 REQUIRED )
target_link_libraries( Burst PUBLIC glfw )