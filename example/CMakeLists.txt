set(
    HEADER_FILES
        TestPresenter.h
)

set(
    SOURCE_FILES
        Source/TestPresenter.cpp
        main.cpp
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

install_burst_resources( Example )
