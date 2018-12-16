#version 150

in vec2 pass_texture;
flat in int pass_effect;

uniform sampler2D Texture;

out vec4 out_Color;

void main() 
{
    vec2 temp = pass_texture;
     //mirroring
    if (pass_effect == 1) {
        temp.x = 1 - temp.x;
    } else if (pass_effect == 2) {
        temp.y = 1 - temp.y;
    }

	// get rgb colours
    vec3 rgb = texture(Texture, temp).rgb;
    out_Color = vec4(rgb.r, rgb.g, rgb.b, 1);
}
