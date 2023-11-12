#version 450

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout( binding = 0 ) uniform sampler2D inImage;

void main()
{
//    vec4 col = texture( inImage, inUV ).rgba;
    outColor = vec4( 1, 0, 0, 1 );
}