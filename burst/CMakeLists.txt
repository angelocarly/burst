set(
    SOURCE_FILES
        Source/Engine.cpp
)

set(
    HEADER_FILES
        Engine.h
)

add_library(
    Burst SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET ${LIBRARY_OUTPUT_NAME} PROPERTY CXX_STANDARD 17 )
include_directories( ${PROJECT_ROOT_DIR} )

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Burst PUBLIC spdlog::spdlog )