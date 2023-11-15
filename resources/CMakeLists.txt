
### Shaders

set(
    GLSL_SOURCE_FILES
        shaders/ScreenRect.vert
        shaders/Sampler.frag
        shaders/Compute.comp
)

if(LINUX)
    set( GLSL_VALIDATOR "/usr/bin/glslangValidator" )
else()
    set( GLSL_VALIDATOR "/usr/local/bin/glslangValidator" )
endif()

foreach( GLSL ${GLSL_SOURCE_FILES} )
    get_filename_component( FILE_NAME ${GLSL} NAME )
    set(
        SPIRV
            "${CMAKE_CURRENT_BINARY_DIR}/shaders/${FILE_NAME}.spv"
    )
    add_custom_command(
            OUTPUT ${SPIRV}
            COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${CMAKE_CURRENT_SOURCE_DIR}/${GLSL} -o ${SPIRV}
            DEPENDS ${GLSL}
            COMMENT "Compiling shader ${FILE_NAME}"
    )
    list( APPEND SPIRV_BINARY_FILES ${SPIRV} )
endforeach( GLSL )

add_custom_target(
    Shaders
        DEPENDS ${SPIRV_BINARY_FILES}
)

add_custom_command(
        TARGET Shaders PRE_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_CURRENT_BINARY_DIR}/shaders
        COMMENT "Creating shader directory"
)

function( install_burst_resources project_name )
    add_dependencies( ${project_name} Shaders )
    add_custom_command(
            TARGET ${project_name} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_BINARY_DIR}/resources $<TARGET_FILE_DIR:${project_name}>/resources
            DEPENDS Shaders
            COMMENT "Creating resources symlink"
    )
endfunction()