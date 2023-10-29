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

set_property( TARGET ${LIBRARY_OUTPUT_NAME} PROPERTY CXX_STANDARD 17 )

include_directories( ${PROJECT_ROOT_DIR} )
