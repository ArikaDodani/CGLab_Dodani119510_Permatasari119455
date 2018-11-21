#version 150


in  vec3 pass_Normal;
// here we are receiving the vertex position and the diffuse color from the fragment shader
// we have referred to the following links to create a blinn phong shader
// https://learnopengl.com/Advanced-Lighting/Advanced-Lighting
// 
in	vec3 diffuseColor1;
in 	vec3 vertPos;					// from vertex position
//in vec4 lightPos;
in vec3 lightPos;					// from light position 
out vec4 out_Color;					// from planets' output colors


precision mediump float;

uniform int mode;

//vec3 lightPos = vec3(0.0,0.0,0.0);
vec3 lightColor = vec3(1.0, 1.0, 1.0);			// vectors for light color
float lightPower = 400.0;				// value for light intensity
vec3 ambientColor = diffuseColor1;
vec3 specColor = vec3(1.0, 1.0, 1.0);			// vectors for specular color
float shininess =  1.0;					// shininess value for the Blinn-Phong
float screenGamma = 2.2; 				// Assume the monitor is calibrated to the sRGB color space

//implementation of fragment shader for the Blinn-Phong model:
void main() {
vec3 lightpos1 = vec3(lightPos);
vec3 normal = pass_Normal;				// passing normal
vec3 lightDir = lightPos - vertPos;			// the light direction is the difference of the light position and vertex position
vec3 viewDir    = normalize(-vertPos);
vec3 halfwayDir = normalize(lightDir + viewDir);
//vec3 specular = lightColor * spec;
float distance = length(lightDir);			// the reflection distance is the length of light direction			
distance = distance * distance * 4 * 3.14;
float specularStrength = 0.5;				// determining specular intensity
// determining specular value from normal, normalized light direction & view direction, shininess
float spec = pow(max(dot(pass_Normal, halfwayDir), 0.0), shininess);
vec3 specular =   lightColor * spec;
float specAngle = max(dot(halfwayDir, pass_Normal), 0.0);
spec = pow(specAngle*spec, 4*shininess);
// getting lambertian value to later determine Blinn-Phong reflection
float lambertian = max(dot(lightDir,pass_Normal), 0.0);

vec3 specular11 = spec * lightColor; 
vec3 colorLinear = ambientColor + (lightColor * lightPower / distance)*((diffuseColor1 * lambertian ) + (specColor * spec ));
//vec3 colorLinear = ambientColor + diffuseColor1 + specular11;
//vec3 colorLinear = ambientColor + diffuseColor1 +(specColor * spec );


out_Color = vec4(colorLinear, 1.0);
//out_Color = vec4(1.0,0.0,0.0, 1.0);
}
