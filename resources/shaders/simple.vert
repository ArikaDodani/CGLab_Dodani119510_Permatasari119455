#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;


//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

//input of a DiffuseColor to see if the matrix works
uniform vec3 diffuseColor;
uniform vec3 origin;
//vec4 origin = vec4(0.0, 0.0, 0.0, 1.0);


vec3 lightColor;				// determining light color
float lightPower;				// determining light intensity
vec3 ambientColor;				// determining ambient color
vec3 specColor;					// determining specular color
float shininess;				// determining the shininess exponent
float screenGamma; 				// determining the screen gamma
// Assume the monitor is calibrated to the sRGB color space

// the values below are sent to the Blinn-Phong fragment shader
out vec3 diffuseColor1;				// sending the diffuse color
out vec3 vertPos;				// sending the vertex position 
out vec3 pass_Normal;
out vec3 lightPos;				// sending the light position

void main(void)
{
	// implementing the Phong shading:
	gl_Position = (ProjectionMatrix  * ViewMatrix * ModelMatrix) * vec4(in_Position, 1.0);
	pass_Normal = (NormalMatrix * vec4(in_Normal, 0.0)).xyz;
	// passing the vertex position here:
	// Referred from the following tutorial https://learnopengl.com/Lighting/Basic-Lighting
	vertPos = vec3(ProjectionMatrix * ViewMatrix * ModelMatrix * vec4(in_Position,1.0));
	diffuseColor1 = diffuseColor;
	lightPos = origin; 
}
