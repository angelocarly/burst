set(
    HEADER_FILES
        Instance.h
)

set(
    SOURCE_FILES
        Source/Instance.cpp
)

add_library(
    Vkt SHARED
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET ${LIBRARY_OUTPUT_NAME} PROPERTY CXX_STANDARD 17 )
include_directories( ${PROJECT_ROOT_DIR} )

### Libraries

# spdlog
find_package( spdlog REQUIRED )
target_link_libraries( Vkt PUBLIC spdlog::spdlog )