#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

// included structs for the models_object
#include "structs.hpp"

//include pointlightNode for colordefinition
#include "PointLightNode.h"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 
using namespace gl;

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <iterator>
#include <list>

#include "scenegraph.h"
#include "Node.h"
#include "texture_loader.hpp"
#include "pixel_data.hpp"

vec4 LightSource;


ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
	,planet_object{}
	,quad_object{}
	,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
	,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
	Node root1 = planetGenerator();
	root = planetColorGenerator(root1);
	initializeShaderPrograms();
	initializeGeometry();
	initializeStarsGeometry();

	//ASSIGNMENT 4 (ADDITIONAL TASK)
	//innitializeSkyBoxGeometry();
	//uploadSkyBox();

	// ASSIGNMENT 5
	initializeFrameBuffer();
	initializeQuadGeometry();
}


ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}


Node ApplicationSolar::planetGenerator() {
	//Node root = Node();

	// here we are adding the children to the sun. Every parameter defines the rotation, translation and the scale of the planets
	root.addChildren(Node("Sun", 1.0f, { 0.0f, 0.0f, 0.0f }, 1.0f));
	root.addChildren(Node("Mercury", 0.3f, { 0.0f, 0.0f, -6.0f }, 1.0f));
	root.addChildren(Node("Venus", 0.4f, { 0.0f, -1.0f, 8.0f }, 1.4f));
	root.addChildren(Node("Earth", 0.5f, { 0.0f, 2.0f, -9.0f }, 0.9f));
	root.addChildren(Node("Mars", 0.4f, { 9.0f, 0.0f, 13.0f }, 0.5f));
	root.addChildren(Node("Jupiter", 0.7f, { 1.0f, 0.0f, 9.0f }, 0.9f));
	root.addChildren(Node("Saturn", 0.6f, { 5.0f, 0.0f, -10.0f }, 0.5f));
	root.addChildren(Node("Uranus", 0.6f, { -5.0f, 0.0f, 11.0f }, 0.5f));

	cout << "scenegraph has been made" << endl;
	return root;
	
}


// uploading all planets and doing all relevant transformations
void ApplicationSolar::uploadPlanets(Node planet_display, int i) const {
	// ASSIGNMENT 4
	//Here we are using the planet shader for passing textures to our planets
	glUseProgram(m_shaders.at("planet").handle);

	// the string is collecting the planet name from the scenegraph and using the name, we are now going to read the texture files and
	//apply them to our planets
	//the texture ID is being collected from the initialize texture program to our planets
	//string filename = planet_display.getName();
	//GLuint texture_object = initializeTexturePrograms(filename, i);

	// here we update the uniforms and send data from the CPU to the GPU
	//int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "texture_object");
	//cout << sampler_location << endl;
	//glUniform1i(sampler_location,i);
	//glUniform1i(glGetUniformLocation(m_shaders.at("planet").handle, "texture_object"), i);


	//activate textures
	//glActiveTexture(GL_TEXTURE0+i);
	//glBindTexture(GL_TEXTURE_2D, texture_object );

	

	//ASSIGNMENT 3
	// here the color is retrieved from the PointLightNode
	vec3 light_color = planet_display.planet_color;
	// the color is being uploaded to the fragment shader
	glUniform3fv(m_shaders.at("planet").u_locs.at("diffuseColor"), 1, value_ptr(light_color));

	if (planet_display.getName() == "Sun") {
		LightSource = { 0.0,0.0, 0.0, 1.0 };
	}
	else {
		LightSource = { 0.0, 0.0, 0.0, 0.0 };
	}
	// the origin is being nultiplied with the view matrix and then being uploaded to the fragment shader
	glm::fmat4 view_matrix = glm::inverse(m_view_transform);
	vec3 origin_position(view_matrix * LightSource);
	glUniform3fv(m_shaders.at("planet").u_locs.at("origin"), 1, value_ptr(origin_position));



	//ASSIGNMENT 1
	// now we are determining the scale of each planet and storing it in a variables
	fvec3 planet_translaton = planet_display.getTranslation();
	float planet_size = planet_display.getDepth();

	// here we are retrieving the rotation
	float planet_rotate = planet_display.getRotation();

	// the variables are being passed into the rotate, scale and the translation function
	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, planet_rotate,0.0f });
	model_matrix = glm::scale(model_matrix, glm::fvec3{ planet_size,planet_size,planet_size });
	model_matrix = glm::translate(model_matrix, planet_translaton);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	//extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normal_matrix));

	 // bind the VAO to draw
	glBindVertexArray(planet_object.vertex_AO);
	// draw bound vertex array using bound shader
	glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

}


void ApplicationSolar::uploadTexturedPlanets(Node planet_display, int i) const {
	// ASSIGNMENT 4
	//Here we are using the planet shader for passing textures to our planets
	glUseProgram(m_shaders.at("planet").handle);

	// the string is collecting the planet name from the scenegraph and using the name, we are now going to read the texture files and
	//apply them to our planets
	//the texture ID is being collected from the initialize texture program to our planets
	string filename = planet_display.getName();


	// we create texture IDs and then bind those IDs to the openGL. We bind the ID's to the 2D texture points. 
	GLuint texture_object;
	glActiveTexture(GL_TEXTURE0 + i);

	// then we generate a memory space for a texture and bind an ID to it
	glGenTextures(1, &texture_object);
	// since textures are objects, they need to be called in a function just like a VAO and VBO.
	// we want to bind the texture as a 2D
	glBindTexture(GL_TEXTURE_2D, texture_object);

	// if the texture gets bigger for the camera
	//magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//minification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	pixel_data image = texture_loader::file(m_resource_path + "textures/" + filename + ".jpg");
	// to create a texture behind the scene. Here we want to generate one texture and then get the ID

	glTexImage2D(GL_TEXTURE_2D, 0, image.channels, image.width, image.height, 0, image.channels, image.channel_type, image.ptr());
	cout << filename << " texture has been created" << endl;


	// here we update the uniforms and send data from the CPU to the GPU
	int sampler_location = glGetUniformLocation(m_shaders.at("planet").handle, "texture_object");
	//cout << sampler_location << endl;
	//glUniform1i(sampler_location,i);
	glUniform1i(glGetUniformLocation(m_shaders.at("planet").handle, "texture_object"), i);


	//activate textures
	glActiveTexture(GL_TEXTURE0 + i);
	glBindTexture(GL_TEXTURE_2D, texture_object);



	//ASSIGNMENT 3
	// here the color is retrieved from the PointLightNode
	vec3 light_color = planet_display.planet_color;
	// the color is being uploaded to the fragment shader
	glUniform3fv(m_shaders.at("planet").u_locs.at("diffuseColor"), 1, value_ptr(light_color));

	if (planet_display.getName() == "Sun") {
		LightSource = { 0.0,0.0, 0.0, 1.0 };
	}
	else {
		LightSource = { 0.0, 0.0, 0.0, 0.0 };
	}
	// the origin is being nultiplied with the view matrix and then being uploaded to the fragment shader
	glm::fmat4 view_matrix = glm::inverse(m_view_transform);
	vec3 origin_position(view_matrix * LightSource);
	glUniform3fv(m_shaders.at("planet").u_locs.at("origin"), 1, value_ptr(origin_position));



	//ASSIGNMENT 1
	// now we are determining the scale of each planet and storing it in a variables
	fvec3 planet_translaton = planet_display.getTranslation();
	float planet_size = planet_display.getDepth();

	// here we are retrieving the rotation
	float planet_rotate = planet_display.getRotation();

	// the variables are being passed into the rotate, scale and the translation function
	glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, planet_rotate,0.0f });
	model_matrix = glm::scale(model_matrix, glm::fvec3{ planet_size,planet_size,planet_size });
	model_matrix = glm::translate(model_matrix, planet_translaton);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	//extra matrix for normal transformation to keep them orthogonal to surface
	glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
	glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(normal_matrix));

	// bind the VAO to draw
	glBindVertexArray(planet_object.vertex_AO);
	// draw bound vertex array using bound shader
	glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

}

void ApplicationSolar::render() const {

	//ASSIGNMENT 5
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);
	glEnable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	renderQuad();


	cout << "rendering..." << endl;
	// bind shader to upload uniforms
	for (int i = 0; i < children_list.size(); i++) {
		Node planet_display = children_list[i];
		uploadPlanets(planet_display,i);

  }


//Stars are drawn here
  // bind shader to upload uniforms
  glUseProgram(star_shaders.at("stars").handle);
  glBindVertexArray(stars.vertex_AO);
  // draw bound vertex array using bound shader
  glDrawArrays(stars.draw_mode, 0, 1000);
// .....................................................................................................



}

//ASSIGNMENT 3
// planet color generator
Node ApplicationSolar::planetColorGenerator(Node root) {

	children_list = root.getChildrenList();

	for (int i = 0; i < children_list.size(); i++){
		Node planet = children_list[i];
		string name= children_list[i].getName();
		float intensity_generator = float((rand() % 10000) / 100);
		children_list[i].planet_color = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ,static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
	}
	return root;
}


void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);

  glUseProgram(skybox_shaders.at("skybox").handle);
  glUseProgram(star_shaders.at("stars").handle);
  glUseProgram(m_shaders.at("planet").handle);

  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniformMatrix4fv(star_shaders.at("stars").u_locs.at("ViewMatrix"),1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniformMatrix4fv(skybox_shaders.at("skybox").u_locs.at("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));


  //ASSIGNMENT 3
  // the origin is uploaded to the fragment shader
  glm::vec4 Lightsource1 = view_matrix * glm::vec4(0.0, 0.0, 0.0, 0.0);
  vec3 origin_position(Lightsource1);
  glUniform3fv(m_shaders.at("planet").u_locs.at("origin"), 1, value_ptr(origin_position));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
	glUseProgram(star_shaders.at("stars").handle);
	glUseProgram(m_shaders.at("planet").handle);
	glUniformMatrix4fv(star_shaders.at("stars").u_locs.at("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(m_view_projection));
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),1, GL_FALSE, glm::value_ptr(m_view_projection));

}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialzsation functions /////////////////////////

// load texture sources
GLuint ApplicationSolar::initializeTexturePrograms(string filename, GLuint index) const{

	//ASSIGNMENT 4
	// we create texture IDs and then bind those IDs to the openGL. We bind the ID's to the 2D texture points. 
	GLuint texture_object;
	glActiveTexture(GL_TEXTURE0 + index);
	
	// then we generate a memory space for a texture and bind an ID to it
	glGenTextures(1, &texture_object);
	// since textures are objects, they need to be called in a function just like a VAO and VBO.
	// we want to bind the texture as a 2D
	glBindTexture(GL_TEXTURE_2D, texture_object);

	// if the texture gets bigger for the camera
	//magnification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	//minification
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	pixel_data image = texture_loader::file(m_resource_path + "textures/" + filename + ".jpg");
	// to create a texture behind the scene. Here we want to generate one texture and then get the ID
	
	glTexImage2D(GL_TEXTURE_2D, 0, image.channels, image.width, image.height, 0, image.channels, image.channel_type, image.ptr());
	cout <<filename <<  " texture has been created" << endl;
	return texture_object;

}

// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container

  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple-final.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple-final.frag"}}});
  
  //loaded the star shaders here
  star_shaders.emplace("stars", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
										 {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}} });

  //ASSIGNMENT 4
  // loading the skybox shader here
  skybox_shaders.emplace("skybox", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/skybox.vert"},
									 {GL_FRAGMENT_SHADER, m_resource_path + "shaders/skybox.frag"}} });
  
 

  // request uniform locations for shader program
star_shaders.at("stars").u_locs["ViewMatrix"] = -1;
star_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;

  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;
  
  //ASSIGNMENT 3
  // the diffusecolor variable is initialized from the fragment shader
  m_shaders.at("planet").u_locs["diffuseColor"] = -1;
  m_shaders.at("planet").u_locs["origin"] = -1;
  
  //ASSIGNMENT 4
  // requesting the uniform locations for the skybox shader program
  skybox_shaders.at("skybox").u_locs["ModelMatrix"] = -1;
  skybox_shaders.at("skybox").u_locs["ViewMatrix"] = -1;
  skybox_shaders.at("skybox").u_locs["ProjectionMatrix"] = -1;


  //ASSIGNMENT 5
  m_shaders.emplace("quad", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/quad.vert"},
									 {GL_FRAGMENT_SHADER, m_resource_path + "shaders/quad.frag"}} });
  // requesting the uniform locations for quad shader
  m_shaders.at("quad").u_locs["texture"] = -1;
  m_shaders.at("quad").u_locs["effect"] = -1;

  cout << "the shaders have been initialized" << endl;
}

//Load stars
void ApplicationSolar::initializeStarsGeometry() {
	//ASSIGNMENT 3
  // .....................................................................................................
  // stars are being initialized here
	GLfloat star_parameters1[6000];
	GLfloat* star_parameters = star_generator(star_parameters1);

	//created a model object for the stars in the header file
	// generate vertex array object for the stars
	glGenVertexArrays(1, &stars.vertex_AO);
	// bind the array for attaching buffers 
	glBindVertexArray(stars.vertex_AO);
	// generate generic buffer
	glGenBuffers(1, &stars.vertex_BO);
	// bind this as an vertex array buffer containing all attributes
	glBindBuffer(GL_ARRAY_BUFFER, stars.vertex_BO);
	// configure currently bound array buffer --
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6000, star_parameters, GL_STATIC_DRAW);
	// activate first attribute on gpu
	glEnableVertexAttribArray(0);
	// first attribute is 3 floats with no offset & stride
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(0));
	// activate second attribute on gpu
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(3 * sizeof(float)));

	// store type of primitive to draw the star
	stars.draw_mode = GL_POINTS;
	stars.num_elements = GLsizei(1000);
	// .....................................................................................................


}

// the following randomized function generates the stars with the random colors at the random positions
GLfloat* ApplicationSolar::star_generator (GLfloat star_p[]) {
	////star position
  // the reference has been taken from the following sourc
	//https://stackoverflow.com/questions/686353/c-random-float-number-generation
  for (int i = 0; i < 3000; i=i+6) {

	  float starpositionx = float(((rand() % 10000) / 100) - 50);
	  float starpositiony = float(((rand() % 10000) / 100) - 50);
	  float starpositionz = float(((rand() % 10000) / 100) - 50);
	
	  //float starpositionx = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 - (-50))));
	  //float starpositiony = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 - (-50))));
	  //float starpositionz = -50 + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (50 - (-50))));
	  star_p[i]= starpositionx;
	  star_p[++i] = starpositiony;
	  star_p[++i] = starpositionz;

	float starcolor_R = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float starcolor_G = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	float starcolor_B = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	star_p[++i] = starcolor_R;
	star_p[++i] = starcolor_G;
	star_p[++i] = starcolor_B;
 }
  cout << "stars have been created" << endl;
	  return star_p;
}


void ApplicationSolar::initializeGeometry() {


  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL | model:: TEXCOORD);
 
  // generate vertex array object
  glGenVertexArrays(1, &planet_object.vertex_AO);
  // bind the array for attaching buffers
  glBindVertexArray(planet_object.vertex_AO);

  // generate generic buffer
  glGenBuffers(1, &planet_object.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, planet_object.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, sizeof(float) * planet_model.data.size(), planet_model.data.data(), GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, model::POSITION.components, model::POSITION.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::POSITION]);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
  // second attribute is 3 floats with no offset & stride
  glVertexAttribPointer(1, model::NORMAL.components, model::NORMAL.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::NORMAL]);

  //ASSIGNMENT 4
  // .....................................................................................................
  //activate the third attribute on gpu
  glEnableVertexAttribArray(2);
  glVertexAttribPointer(2, model::TEXCOORD.components, model::TEXCOORD.type, GL_FALSE, planet_model.vertex_bytes, planet_model.offsets[model::TEXCOORD]);
  // .....................................................................................................

   // generate generic buffer
  glGenBuffers(1, &planet_object.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, planet_object.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, model::INDEX.size * planet_model.indices.size(), planet_model.indices.data(), GL_STATIC_DRAW);

  // store type of primitive to draw
  planet_object.draw_mode = GL_TRIANGLES;
  // transfer number of indices to model object 
  planet_object.num_elements = GLsizei(planet_model.indices.size());
  cout << "geometries have been created" << endl;

}


//ASSIGNMENT 4 (ADDITIONAL TASK)
void ApplicationSolar::innitializeSkyBoxGeometry()
{


	// ASSIGNMENT 4
	// vertices for the skybox
	// Creating a cube here *
	// the vertices have been taken from here: https://learnopengl.com/Advanced-OpenGL/Cubemaps
	float skyboxVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	;	// generate vertex array object
	glGenVertexArrays(1, &skybox_vao);
	glBindVertexArray(skybox_vao);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, NULL);

	glGenBuffers(1, &skybox_vbo);
	glBindBuffer(GL_ARRAY_BUFFER, skybox_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * 36 * sizeof(float), &skyboxVertices, GL_STATIC_DRAW);

	cout << "the skybox geometry has been created" << endl;
}

// ASSIGNMENT 4 (ADDITIONAL TASK) 
//reference: https://learnopengl.com/Advanced-OpenGL/Cubemaps
// http://antongerdelan.net/opengl/cubemaps.html
void ApplicationSolar::uploadSkyBox() 
{

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	pixel_data data;
	string texture_faces[6] = { "cwd_rt", "cwd_lf", "cwd_up", "cwd_dn" , "cwd_bk", "cwd_ft" };
	for (GLuint i = 0; i < texture_faces->size(); i++)
	{
		data = texture_loader::file(m_resource_path + "textures/skybox/" + texture_faces[i] + ".jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data.channels, data.width, data.height, 0, data.channels, GL_UNSIGNED_BYTE, data.ptr());
		cout << data.height << endl;
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);


	glDepthMask(GL_FALSE);

	glUseProgram(skybox_shaders.at("skybox").handle);

	glm::fmat4 model_matrix = glm::scale(model_matrix, glm::fvec3{ 30.0, 30.0, 30.0 });
	glUniformMatrix4fv(skybox_shaders.at("skybox").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));

	glm::fmat4 view_matrix = glm::scale(view_matrix, glm::fvec3{ 30.0, 30.0, 30.0 });
	glUniformMatrix4fv(skybox_shaders.at("skybox").u_locs.at("ViewMatrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));

	glm::fmat4 projection_matrix = glm::scale(projection_matrix, glm::fvec3{ 30.0, 30.0, 30.0 });
	glUniformMatrix4fv(skybox_shaders.at("skybox").u_locs.at("ProjectionMatrix"), 1, GL_FALSE, glm::value_ptr(projection_matrix));



	int sampler_location = glGetUniformLocation(skybox_shaders.at("skybox").handle, "skybox_texture");
	//cout << sampler_location << endl;
	//glUniform1i(sampler_location,i);
	glUniform1i(sampler_location, 0);
	
	//glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(skybox_vao);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glDepthMask(GL_TRUE);

	cout << "the skybox has been uploaded" << endl;

}

//ASSIGNMENT 4
GLuint ApplicationSolar::loadCubemap() const {
	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

	pixel_data data;
	string texture_faces[6] = { "cwd_rt", "cwd_lf", "cwd_up", "cwd_dn" , "cwd_bk", "cwd_ft" };
	for (GLuint i = 0; i < texture_faces->size(); i++)
	{
		data = texture_loader::file(m_resource_path + "textures/skybox/" + texture_faces[i] + ".jpg");
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, data.channels, data.width, data.height, 0, data.channels, GL_UNSIGNED_BYTE, data.ptr());
		//cout << data.height << endl;
	}
	
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return textureID;

}


//ASSIGNMENT 5
void ApplicationSolar::initializeFrameBuffer() {


	// the following tutorial was referenced: https://learnopengl.com/Advanced-OpenGL/Framebuffers
	int width = 400;
	int height = 600;

	// first we are creating and initializing the frame buffer object
	glGenFramebuffers(1, &fbo_handle);
	//By binding to the GL_FRAMEBUFFER target all the next read and write framebuffer operations will affect the currently bound framebuffer. 
	glBindFramebuffer(GL_FRAMEBUFFER, fbo_handle);

	// next, we have to attach atleast one buffer  and have atleast one color attachment to the buffer

	// texture attachment
	// generate texture
	glActiveTexture(GL_TEXTURE1);
	glGenTextures(1, &tex_handle);
	glBindTexture(GL_TEXTURE_2D, tex_handle);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	//Now that we've created a texture we need to actually attach it to the framebuffer
	// attaching below to currently bound framebuffer object
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, tex_handle, 0);

	//Renderbuffer objects store all the render data directly into their buffer without any conversions to texture-specific formats
	// this makes them faster as a writeable storage medium
	// now we generate a render buffer
	glGenRenderbuffers(1, &rb_handle);
	// we bind the render buffer object 
	glBindRenderbuffer(GL_RENDERBUFFER, rb_handle);
	// Creating a depth and stencil renderbuffer object
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, width, height);
	// Now we attach the renderbuffer object
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rb_handle);




	GLenum draw_buffer[1] = { GL_COLOR_ATTACHMENT0 };
	glDrawBuffers(1, draw_buffer);
}

void ApplicationSolar::renderQuad() const {
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT);
	glBindVertexArray(quad_object.vertex_AO);
	shader_program quad_program = m_shaders.at("quad");
	glUseProgram(quad_program.handle);
	glActiveTexture(GL_TEXTURE2);
	glUniform1i(quad_program.u_locs.at("texture"), 2);
	glUniform1iv(quad_program.u_locs.at("effect"), 1, &effect);
	glDrawArrays(quad_object.draw_mode, NULL, quad_object.num_elements);
}

void ApplicationSolar::initializeQuadGeometry() {
	std::vector<GLfloat> quad{
		-1.0, -1.0, 0.0,
		1.0, -1.0, 0.0,
		-1.0,  1.0, 0.0,
		1.0,  1.0, 0.0
	};

	glGenBuffers(1, &quad_object.vertex_BO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_object.vertex_BO);

	// buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * quad.size(), quad.data(), GL_STATIC_DRAW);

	// vertex array
	glGenVertexArrays(1, &quad_object.vertex_AO);
	glBindVertexArray(quad_object.vertex_AO);
	glBindBuffer(GL_ARRAY_BUFFER, quad_object.vertex_BO);

	// attribute array
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_TRUE, sizeof(float) * 3, 0);
	quad_object.draw_mode = GL_TRIANGLE_STRIP;
	quad_object.num_elements = GLsizei(quad.size() / 3);
}



///////////////////////////// callback functions for window events ////////////

// handle key input
void ApplicationSolar::keyCallback(int key, int action, int mods) {
  if (key == GLFW_KEY_W  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, -0.1f});
    uploadView();
  }
  else if (key == GLFW_KEY_S  && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
    m_view_transform = glm::translate(m_view_transform, glm::fvec3{0.0f, 0.0f, 0.1f});
	uploadView();
  }
  else if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	  m_view_transform = translate(m_view_transform, fvec3{ -0.2f, 0.0f, 0.0f });
	  uploadView();
  }
  else if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
	  m_view_transform = translate(m_view_transform, fvec3{ 0.2f, 0.0f, 0.0f });
	  uploadView();
  }
  

}

//handle delta mouse movement input
void ApplicationSolar::mouseCallback(double pos_x, double pos_y) {
  // mouse handling
}

//handle resizing
void ApplicationSolar::resizeCallback(unsigned width, unsigned height) {
  // recalculate projection matrix for new aspect ration
  m_view_projection = utils::calculate_projection_matrix(float(width) / float(height));
  // upload new projection matrix
  uploadProjection();
}


// exe entry point
int main(int argc, char* argv[]) {
  Application::run<ApplicationSolar>(argc, argv, 3, 2);

}