#include "application_solar.hpp"
#include "window_handler.hpp"

#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

// included structs for the models_object
#include "structs.hpp"


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



ApplicationSolar::ApplicationSolar(std::string const& resource_path)
 :Application{resource_path}
	,planet_object{}
	,m_view_transform{glm::translate(glm::fmat4{}, glm::fvec3{0.0f, 0.0f, 4.0f})}
	,m_view_projection{utils::calculate_projection_matrix(initial_aspect_ratio)}
{
  initializeGeometry();
  //call the planets here to run the application smoothly
  initializeShaderPrograms();
}

ApplicationSolar::~ApplicationSolar() {
  glDeleteBuffers(1, &planet_object.vertex_BO);
  glDeleteBuffers(1, &planet_object.element_BO);
  glDeleteVertexArrays(1, &planet_object.vertex_AO);
}

void ApplicationSolar::render() const {

	// Here we have initialized the scene graph
	scenegraph SolarSystem = scenegraph();
	// here the plants are being initialized. We first initialized the root node i.e the sun
	Node sun = Node("sun", 1.0f, { 0.0f, 0.0f, 0.0f },0.0f);
	// here we are adding the children to the sun. Every parameter defines the rotation, translation and the scale of the planets
	sun.addChildren(Node("Mercury", 0.3f, { 2.0f, 0.0f, 0.0f }, 1.0f));
	sun.addChildren(Node("Venus", 0.4f, { 4.0f, 5.0f, 5.0f }, 1.4f));
	sun.addChildren(Node("Earth", 0.5f, { 6.0f, -7.0f, 6.0f }, 0.9f));
	sun.addChildren(Node("Mars", 0.4f, { 8.0f, 9.0f, 6.0f },0.5f));
	sun.addChildren(Node("Jupiter", 0.7f, { 10.0f, 8.0f, -9.0f }, 0.9f));
	sun.addChildren(Node("Saturn", 0.6f, { -12.0f, 10.0f, 10.0f }, 0.5f));
	sun.addChildren(Node("Uranus", 0.6f, { 14.0f, -11.0f, 11.0f }, 0.5f));
	sun.addChildren(Node("Neptune", 0.4f, { 16.0f, -12.0f, -12.0f }, 0.5f));
	//the first variable is the planet name, second is the float type of the planet size, third is the translation vector and the fourth is the rotation
	//here we are storing the translation vector in a variable
	fvec3 sun_translation = sun.getTranslation();


  // bind shader to upload uniforms
  glUseProgram(m_shaders.at("planet").handle);

  // the variables are being passed into the rotate, scale and the translation function
  glm::fmat4 model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{ 0.0f, 1.0f, 0.0f });
  model_matrix = glm::translate(model_matrix, sun_translation);
  // here we are collecting the size of the sun
  float ssize = sun.getDepth();
  model_matrix = glm::scale(model_matrix, glm::fvec3{ ssize,ssize,ssize });
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
                     1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  glm::fmat4 normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
                     1, GL_FALSE, glm::value_ptr(normal_matrix));
  glUniformMatrix4fv(star_shaders.at("stars").u_locs.at("NormalMatrix"),
	  1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);

  // .....................................................................................................
  //Stars are here.
	// bind shader to upload uniforms
  glBindVertexArray(stars.vertex_AO);
  glUseProgram(star_shaders.at("stars").handle);
  model_matrix = glm::translate(model_matrix, {5.0f,5.0f,5.0f});
  model_matrix = glm::translate(model_matrix, { 6.0f,6.0f,6.0f });
 // glUniformMatrix4fv(star_shaders.at("stars").u_locs.at("ModelMatrix"),
//	  1, GL_FALSE, glm::value_ptr(model_matrix));
  // draw bound vertex array using bound shader
  glDrawElements(stars.draw_mode, stars.num_elements, model::INDEX.type, NULL);

  // .....................................................................................................
  
  // all the nodes with their respective transformations have been looped here. First we are storing the array of object in a list variable
  list<Node> children_list = sun.getChildrenList();
  
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
	model_matrix = glm::rotate(glm::fmat4{}, float(glfwGetTime()), glm::fvec3{0.0f, planet_rotate,0.0f });
	model_matrix = glm::scale(model_matrix, glm::fvec3{ planet_size,planet_size,planet_size });
  model_matrix = glm::translate(model_matrix, planet_translaton);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ModelMatrix"),
	  1, GL_FALSE, glm::value_ptr(model_matrix));

  // extra matrix for normal transformation to keep them orthogonal to surface
  normal_matrix = glm::inverseTranspose(glm::inverse(m_view_transform) * model_matrix);
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("NormalMatrix"),
	  1, GL_FALSE, glm::value_ptr(normal_matrix));

  // bind the VAO to draw
  glBindVertexArray(planet_object.vertex_AO);

  // draw bound vertex array using bound shader
  glDrawElements(planet_object.draw_mode, planet_object.num_elements, model::INDEX.type, NULL);
  
  }
}


void ApplicationSolar::uploadView() {
  // vertices are transformed in camera space, so camera transform must be inverted
  glm::fmat4 view_matrix = glm::inverse(m_view_transform);
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ViewMatrix"),
                     1, GL_FALSE, glm::value_ptr(view_matrix));
}

void ApplicationSolar::uploadProjection() {
  // upload matrix to gpu
  glUniformMatrix4fv(m_shaders.at("planet").u_locs.at("ProjectionMatrix"),
                     1, GL_FALSE, glm::value_ptr(m_view_projection));
}

// update uniform locations
void ApplicationSolar::uploadUniforms() { 
  // bind shader to which to upload unforms
  glUseProgram(m_shaders.at("planet").handle);
  // upload uniform values to new locations
  uploadView();
  uploadProjection();
}

///////////////////////////// intialisation functions /////////////////////////
// load shader sources
void ApplicationSolar::initializeShaderPrograms() {
  // store shader program objects in container

  m_shaders.emplace("planet", shader_program{{{GL_VERTEX_SHADER,m_resource_path + "shaders/simple.vert"},
                                           {GL_FRAGMENT_SHADER, m_resource_path + "shaders/simple.frag"}}});

  //loaded the star shaders here
  star_shaders.emplace("stars", shader_program{ {{GL_VERTEX_SHADER,m_resource_path + "shaders/vao.vert"},
										 {GL_FRAGMENT_SHADER, m_resource_path + "shaders/vao.frag"}} });
  // request uniform locations for shader program
star_shaders.at("stars").u_locs["NormalMatrix"] = -1;
star_shaders.at("stars").u_locs["ModelMatrix"] = -1;
star_shaders.at("stars").u_locs["ViewMatrix"] = -1;
star_shaders.at("stars").u_locs["ProjectionMatrix"] = -1;



  // request uniform locations for shader program
  m_shaders.at("planet").u_locs["NormalMatrix"] = -1;
  m_shaders.at("planet").u_locs["ModelMatrix"] = -1;
  m_shaders.at("planet").u_locs["ViewMatrix"] = -1;
  m_shaders.at("planet").u_locs["ProjectionMatrix"] = -1;


}

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

//-------------------------------------------------------
//-------------------------------------------------------
  //created a model object for the stars in the header file
  // generate vertex array object for the stars
  glGenVertexArrays(1, &stars.vertex_AO);
  // bind the array for attaching buffers 
  glBindVertexArray(stars.vertex_AO);
  // generate generic buffer
  glGenBuffers(1, &stars.vertex_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ARRAY_BUFFER, stars.vertex_BO);
  // configure currently bound array buffer
  glBufferData(GL_ARRAY_BUFFER, 1.0, star_parameters, GL_STATIC_DRAW);

  // activate first attribute on gpu
  glEnableVertexAttribArray(0);
  // first attribute is 3 floats with no offset & stride
  glVertexAttribPointer(0, 1.0, GL_FLOAT, GL_FALSE, 1.0, *star_parameters);
  // activate second attribute on gpu
  glEnableVertexAttribArray(1);
 
  // generate generic buffer
  glGenBuffers(1, &stars.element_BO);
  // bind this as an vertex array buffer containing all attributes
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stars.element_BO);
  // configure currently bound array buffer
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, 1.0, star_parameters, GL_STATIC_DRAW);

  // store type of primitive to draw
  stars.draw_mode = GL_POINTS;
  stars.num_elements = GLsizei(20);


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