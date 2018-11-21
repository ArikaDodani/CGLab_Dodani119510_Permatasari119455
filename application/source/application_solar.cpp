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
#include "scenegraph.h"
#include "Node.h"

Node root;
vec4 LightSource;


struct starvalues {
	float positions[3];
	float colors[3];

};

ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
	,planet_object{}
	,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
	,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}


Node ApplicationSolar::planetGenerator() const {
	Node root = Node();
	// here we are adding the children to the sun. Every parameter defines the rotation, translation and the scale of the planets
	root.addChildren(Node("Sun", 1.0f, { 0.0f, 0.0f, 0.0f }, 1.0f));
	root.addChildren(Node("Mercury", 0.3f, { 0.0f, 0.0f, -6.0f }, 1.0f));
	root.addChildren(Node("Venus", 0.4f, { 0.0f, -1.0f, 8.0f }, 1.4f));
	root.addChildren(Node("Earth", 0.5f, { 0.0f, 2.0f, -9.0f }, 0.9f));
	root.addChildren(Node("Mars", 0.4f, { 9.0f, 0.0f, 13.0f }, 0.5f));
	root.addChildren(Node("Jupiter", 0.7f, { 1.0f, 0.0f, 9.0f }, 0.9f));
	root.addChildren(Node("Saturn", 0.6f, { 5.0f, 0.0f, -10.0f }, 0.5f));
	root.addChildren(Node("Uranus", 0.6f, { -5.0f, 0.0f, 11.0f }, 0.5f));

	return root;

}
//ASSIGNMENT 3
Node ApplicationSolar::planetColorGenerator(Node root) const {
	//the list of Nodes is being passed here and in every traversal, the light color and the light intensity is being defined
	list<Node> children_list = root.getChildrenList();

	for (list<Node>::iterator children_list_iterator = children_list.begin(); children_list_iterator != children_list.end(); ++children_list_iterator) {
		Node planet = *children_list_iterator;
		if (planet.getName() == "Sun")
		{
			planet.planetColor.setLightColor({ 1.0,0.0,1.0 });
			planet.planetColor.setLightIntensity(50.0);
		}
		if (planet.getName() == "Mercury")
		{
			planet.planetColor.setLightColor({ 0.0,0.0,1.0 });
			planet.planetColor.setLightIntensity(10.0);
		}
		if (planet.getName() == "Venus")
		{
			planet.planetColor.setLightColor({ 1.0,1.0,1.0 });
			planet.planetColor.setLightIntensity(30.0);
		}

		if (planet.getName() == "Earth")
		{
			planet.planetColor.setLightColor({ 1.0,1.0,1.0 });
			planet.planetColor.setLightIntensity(50.0);
		}

		if (planet.getName() == "Mars")
		{
			planet.planetColor.setLightColor({ 1.0,1.0,1.0 });
			planet.planetColor.setLightIntensity(50.0);
		}

		if (planet.getName() == "Jupiter")
		{
			planet.planetColor.setLightColor({ 0.0,0.0,1.0 });
			planet.planetColor.setLightIntensity(50.0);
		}
		if (planet.getName() == "Saturn")
		{
			planet.planetColor.setLightColor({ 1.0,1.0,0.0 });
			planet.planetColor.setLightIntensity(50.0);
		}

		if (planet.getName() == "Uranus")
		{
			planet.planetColor.setLightColor({ 1.0,1.0,0.0 });
			planet.planetColor.setLightIntensity(50.0);
		}

		if (planet.getName() == "Neptune")
		{
			planet.planetColor.setLightColor({ 1.0,0.0,1.0 });
			planet.planetColor.setLightIntensity(50.0);
		}
	}

	return root;

}


void ApplicationSolar::render() const {


// here the plants are being initialized. The planets are being created in the planetGenerator function
Node root1 = planetGenerator();
// the follwing function is made to generate the colors at the random value between 0.0 and 1 for the planets
Node root = planetColorGenerator(root1);

// all the nodes with their respective transformations have been looped here. First we are storing the array of object in a list variable
list<Node> children_list = root.getChildrenList();

// we are using the iterator to traverse the list to find the planets and their respective properties. We are indexing children and rendering them in a loop
for (list<Node>::iterator children_list_iterator = children_list.begin(); children_list_iterator != children_list.end(); ++children_list_iterator) {
// bind shader to upload uniforms
glUseProgram(m_shaders.at("planet").handle);

  // the said planet is being stored in a variable. the properties are retrived by the iterator pointer
Node planet_display = *children_list_iterator;
// now we are determining the scale of each planet and storing it in a variables
fvec3 planet_translaton = planet_display.getTranslation();
float planet_size = planet_display.getDepth();
// here we are retrieving the rotation
float planet_rotate = planet_display.getRotation();

  // the variables are being passed into the rotate, scale and the translation function
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, planet_rotate,0.0f });
	model_matrix = glm::scale(model_matrix, glm::fvec3{ planet_size,planet_size,planet_size });
  model_matrix = glm::translate(model_matrix, planet_translaton);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model_matrix));


 //extra matrix for normal transformation to keep them orthogonal to surface
 glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
 glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),1, GL_FALSE, glm::value_ptr(normal_matrix));

 //ASSIGNMENT 3
 // .....................................................................................................
 // here the color is retrieved from the PointLightNode
 vec3 color_generator = { static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX) };
 vec3 light_color = vec3(planet_display.planetColor.getLightColor());
//float light_intensity = float(planet_display.planetColor.lightIntensity);
 //cout << light_intensity << endl;
 // the color is being uploaded to the fragment shader
 glUniform3fv(m_shaders.at("planet").u_locs.at("diffuseColor"), 1, value_ptr(color_generator));

 if (planet_display.getName() == "Sun") {
	 LightSource = {0.0,0.0, 0.0, 0.0};
 }
 else {
	 LightSource = {0.0, 0.0, 0.0, 1.0};
 }
 // the origin is being nultiplied with the view matrix and then being uploaded to the fragment shader
 glm::fmat4 view_matrix = glm::inverse(m_view_transform);
 vec3 origin_position(view_matrix * LightSource);
 glUniform3fv(m_shaders.at("planet").u_locs.at("origin"), 1, value_ptr(origin_position));
 // .....................................................................................................



  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);
  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  
  }

 // .....................................................................................................
//Stars are drawn here
  // bind shader to upload uniforms
  glUseProgram(star_shaders.at("stars").handle);
  glBindVertexArray(stars.vertex_AO);
  // draw bound vertex array using bound shader
  glDrawArrays(stars.draw_mode, 0, 1000);
// .....................................................................................................


}

// .....................................................................................................
// planet color generator
//Node ApplicationSolar::planetColorGenerator(Node root) const {
//
//	list<Node> children_list = root.getChildrenList();
//
//	for (list<Node>::iterator children_list_iterator = children_list.begin(); children_list_iterator != children_list.end(); ++children_list_iterator) {
//		Node planet = *children_list_iterator;
//		float intensity_generator = float((rand() % 10000) / 100);
//		// the vec3 with random values is being generated here. The sun and all its planets are being traversed and the light intensity and its color is being input here
//		vec3 color_generator = {static_cast <float> (rand()) / static_cast <float> (RAND_MAX),static_cast <float> (rand()) / static_cast <float> (RAND_MAX) ,static_cast <float> (rand()) / static_cast <float> (RAND_MAX)};
//		planet.planetColor.setLightColor(color_generator);
//		planet.planetColor.setLightIntensity(intensity_generator);
//	}
//	
//	return root;
//}


void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);


  glUseProgram(star_shaders.at("stars").handle);
  glUseProgram(m_shaders.at("planet").handle);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),1, GL_FALSE, glm::value_ptr(view_matrix));
  glUniformMatrix4fv(star_shaders.at("stars").u_locs.at("ViewMatrix"),1, GL_FALSE, glm::value_ptr(view_matrix));


  //ASSIGNMENT 3
  // .....................................................................................................
  // the origin is uploaded to the fragment shader
  glm::vec4 Lightsource1 = view_matrix * glm::vec4(0.0, 0.0, 0.0, 0.0);
  vec3 origin_position(Lightsource1);
  glUniform3fv(m_shaders.at("planet").u_locs.at("origin"), 1, value_ptr(origin_position));
  // .....................................................................................................
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
  // bind shader to which to upload uniforms
  
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialzsation functions /////////////////////////

// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container

  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple-final.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple-final.frag"}}});
  
  //loaded the star shaders here
  star_shaders.emplace("stars", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
										 {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}} });
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
  // .....................................................................................................
  m_shaders.at("planet").u_locs["diffuseColor"] = -1;
  m_shaders.at("planet").u_locs["origin"] = -1;
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
	  return star_p;
}
// .....................................................................................................

// load models
void ApplicationSolar::initializeGeometry() {
  model planet_model = model_loader::obj(m_resource_path + "models/sphere.obj", model::NORMAL);
 
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
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 6, (GLvoid*)(3* sizeof(float)));
	
  // store type of primitive to draw the star
  stars.draw_mode = GL_POINTS;
  stars.num_elements = GLsizei(1000);
  // .....................................................................................................

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