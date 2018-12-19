#version 150
#extension GL_ARB_explicit_attrib_location : require

// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec2 tex_coords;

uniform int mirror;
uniform int grayscale;
uniform int blur;



flat out int pass_mirror;
flat out int pass_grayscale;
flat out int pass_blur;

out vec2 pass_texcoords;


void main(void)
{
    gl_Position = vec4(in_Position, 1.0);
    pass_mirror = mirror;
    pass_grayscale =grayscale;
    pass_blur = blur;
    pass_texcoords = tex_coords;
}