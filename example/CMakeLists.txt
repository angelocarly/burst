set(
    SOURCE_FILES
        main.cpp
)

set(
    HEADER_FILES
)

add_executable(
    Example
        ${SOURCE_FILES}
        ${HEADER_FILES}
)

set_property( TARGET Example PROPERTY CXX_STANDARD 17 )

## Project libraries
target_link_libraries(
    Example PUBLIC
        Burst
)

include_directories( ${PROJECT_ROOT_DIR} )
