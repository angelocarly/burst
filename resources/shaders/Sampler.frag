#version 450

layout( location = 0 ) in vec2 inUV;

layout( location = 0 ) out vec4 outColor;

layout( binding = 0 ) uniform usampler2D inImage;

void main()
{
    int c = int( texture( inImage, inUV ).r );

    int red = c % 256;
    int green = ( c / 256 ) % 256;
    int blue = ( c / 256 / 256  ) % 256;
    vec3 color = vec3( red / 255.0f, green / 255.0f, blue / 255.0f );
    outColor = vec4( color, 1.0f );
}