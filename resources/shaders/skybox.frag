#version 150

in  vec3 skybox_tex_coords;
out vec4 out_Color;
uniform samplerCube skybox_texture;

void main() 
{
	out_Color = texture(skybox_texture, skybox_tex_coords);
}
