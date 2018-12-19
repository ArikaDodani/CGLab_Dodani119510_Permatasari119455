#version 150


in  vec3 pass_Normal;
// here we are receiving the vertex position and the diffuse color from the fragment shader
// we have referred to the following links to create a blinn phong shader
// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
// 
in	vec3 diffuseColor1;
in 	vec3 vertPos;					            // from vertex position
in vec3 lightPos;				            	// from light position 

in vec2 tex_coords1;                            // texture coordinates
in vec3 skybox_tex_coords;

out vec4 out_Color;					            // from planets' output colors
out vec4 skybox1; 

// A sampler here samples the color data from the image loaded or bound.
uniform sampler2D texture_object;

precision mediump float;


vec3 lightColor = vec3(1.0, 1.0, 1.0);			// vectors for light color
float lightPower = 400.0;				        // value for light intensity
vec3 ambientColor = diffuseColor1;
vec3 specColor = vec3(1.0, 1.0, 1.0);			// vectors for specular color
float shininess =  1.0;					        // shininess value for the Blinn-Phong
//float screenGamma = 2.2; 				        // Assume the monitor is calibrated to the sRGB color space

//implementation of fragment shader for the Blinn-Phong model:
void main() {
vec3 lightpos1 = vec3(lightPos);
vec3 normal = pass_Normal;				         // passing normal
vec3 lightDir = lightPos - vertPos;			     // the light direction is the difference of the light position and vertex position
vec3 viewDir    = normalize(-vertPos);
vec3 halfwayDir = normalize(lightDir + viewDir);
float distance = length(lightDir);			     // the reflection distance is the length of light direction			
distance = distance * distance * 4 * 3.14;
// determining specular value from normal, normalized light direction & view direction, shininess
float spec = pow(max(dot(pass_Normal, halfwayDir), 0.0), shininess);
vec3 specular =   lightColor * spec;
float lambertian = max(dot(lightDir,pass_Normal), 0.0);

vec3 colorLinear = ambientColor + (lightColor * lightPower / distance)*((diffuseColor1 * lambertian ) + (specColor * spec ));


//out_Color = vec4(colorLinear, 1.0);
out_Color = vec4(texture(texture_object, tex_coords1)) * vec4(colorLinear, 1.0);
//out_Color = vec4(vec3(texture(planet_texture0,tex_coords1)), 1);
//colour_from_tex = texture(planet_texture0, tex_coords1) * vec4(colorLinear, 1.0);
}
