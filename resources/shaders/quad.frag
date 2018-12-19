#version 150

in vec2 pass_texture;
flat in int pass_mirror;
flat in int pass_grayscale;
flat in int pass_blur;
in vec2 pass_texcoords;

uniform sampler2D Texture;


out vec4 out_Color;

const float offset = 1.0 / 300.0; 

void main() 
{
    vec2 TexCoords = pass_texcoords;  

    // offset for the kernel effect to implement blur
    vec2 offsets[9] = vec2[](
        vec2(-offset,  offset), // top-left
        vec2( 0.0f,    offset), // top-center
        vec2( offset,  offset), // top-right
        vec2(-offset,  0.0f),   // center-left
        vec2( 0.0f,    0.0f),   // center-center
        vec2( offset,  0.0f),   // center-right
        vec2(-offset, -offset), // bottom-left
        vec2( 0.0f,   -offset), // bottom-center
        vec2( offset, -offset)  // bottom-right    
    );

    float kernel[9] = float[](
        1.0 / 16, 2.0 / 16, 1.0 / 16,
        2.0 / 16, 4.0 / 16, 2.0 / 16,
        1.0 / 16, 2.0 / 16, 1.0 / 16  
    );
    
    vec3 sampleTexture[9];
    // Assignment 5 (Additional task)

    // blur effect 

    if (pass_blur == 1) {
        for(int i = 0; i < 9; i++){
            sampleTexture[i] = vec3(texture(Texture, TexCoords.st + offsets[i]));
        }
        vec3 col = vec3(0.0);
        for(int i = 0; i < 9; i++) {
            col += sampleTexture[i] * kernel[i]; }
    
        out_Color = vec4(col, 1.0);
    }


     //mirroring
    else if (pass_mirror == 1) {
        TexCoords.x = 1 - TexCoords.x;
        vec3 rgb = texture(Texture, TexCoords).rgb;
        out_Color = vec4(rgb.r, rgb.g, rgb.b, 1);

    } else if (pass_mirror == 2) {
        TexCoords.y = 1 - TexCoords.y;
        vec3 rgb = texture(Texture, TexCoords).rgb;
        out_Color = vec4(rgb.r, rgb.g, rgb.b, 1);

    }

    // grayscale
    else if (pass_grayscale == 1)
    {
        vec3 rgb = texture(Texture, TexCoords).rgb;
        float average = 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b;
        out_Color = vec4(average, average, average, 1.0);
    }

    else {
        vec3 rgb = texture(Texture, TexCoords).rgb;
        out_Color = vec4(rgb.r, rgb.g, rgb.b, 1);
    }

	// get rgb colours

}
