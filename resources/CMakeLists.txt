
### Shaders

function( compile_shaders PROJECT INPUT_DIRECTORY OUTPUT_DIRECTORY GLSL_SOURCE_FILES )

    set( SHADERTARGET "${PROJECT}_Shaders" )

    if(LINUX)
        set( GLSL_VALIDATOR "/usr/bin/glslangValidator" )
    else()
        set( GLSL_VALIDATOR "/usr/local/bin/glslangValidator" )
    endif()

    add_custom_command(
            OUTPUT ${OUTPUT_DIRECTORY}
            COMMAND ${CMAKE_COMMAND} -E make_directory ${OUTPUT_DIRECTORY}
    )

    # Project shaders
    foreach( GLSL ${GLSL_SOURCE_FILES} )

        get_filename_component( FILE_NAME ${GLSL} NAME )
        set(
                SPIRV
                "${OUTPUT_DIRECTORY}/${FILE_NAME}.spv"
        )
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${INPUT_DIRECTORY}/${GLSL} -o ${SPIRV}
                DEPENDS ${INPUT_DIRECTORY}/${GLSL}
                COMMENT "Compiling shader ${FILE_NAME}"
        )
        list( APPEND SPIRV_BINARY_FILES ${SPIRV} )
    endforeach( GLSL )

    # Burst shaders
    set(
        GLSL_BURST_SOURCE_FILES
            shaders/ScreenRect.vert
    )
    set( INPUT_DIRECTORY "${BURST_RESOURCES_DIR}" )
    foreach( GLSL ${GLSL_BURST_SOURCE_FILES} )
        get_filename_component( FILE_NAME ${GLSL} NAME )
        set(
            SPIRV
                "${OUTPUT_DIRECTORY}/${FILE_NAME}.spv"
        )
        add_custom_command(
                OUTPUT ${SPIRV}
                COMMAND ${GLSL_VALIDATOR} --target-env vulkan1.0 -V ${INPUT_DIRECTORY}/${GLSL} -o ${SPIRV}
                DEPENDS ${INPUT_DIRECTORY}/${GLSL}
                COMMENT "Compiling shader ${FILE_NAME}"
        )
        list( APPEND SPIRV_BINARY_FILES ${SPIRV} )
    endforeach( GLSL )

    add_custom_target(
            ${SHADERTARGET}
            DEPENDS ${OUTPUT_DIRECTORY} ${SPIRV_BINARY_FILES}
    )

    add_dependencies( ${PROJECT} ${SHADERTARGET} )
endfunction()
