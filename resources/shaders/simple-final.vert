#version 150
#extension GL_ARB_explicit_attrib_location : require
// vertex attributes of VAO
layout(location = 0) in vec3 in_Position;
layout(location = 1) in vec3 in_Normal;
layout(location = 2) in vec2 tex_coords;


//Matrix Uniforms as specified with glUniformMatrix4fv
uniform mat4 ModelMatrix;
uniform mat4 ViewMatrix;
uniform mat4 ProjectionMatrix;
uniform mat4 NormalMatrix;

//input of a DiffuseColor to see if the matrix works
uniform vec3 diffuseColor;
uniform vec3 origin;


// Assume the monitor is calibrated to the sRGB color space

// the values below are sent to the Blinn-Phong fragment shader
out vec3 diffuseColor1;				// sending the diffuse color
out vec3 vertPos;				// sending the vertex position 
out vec3 pass_Normal;
out vec3 lightPos;				// sending the light position
out vec2 tex_coords1;
out vec3 skybox_tex_coords;

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

	tex_coords1 = tex_coords;
	skybox_tex_coords = in_Position;
}
