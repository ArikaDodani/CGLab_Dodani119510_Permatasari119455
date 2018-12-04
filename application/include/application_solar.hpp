#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "Node.h"
#include "model.hpp"
#include "structs.hpp"
#include <tuple>
#include <string>
#include <glm/glm.hpp>
#include "PointLightNode.h"
#include "pixel_data.hpp"

using namespace gl;

// gpu representation of model
class ApplicationSolar : public Application {
public:
	// allocate and initialize objects
	ApplicationSolar(std::string const& resource_path);
	// free allocated objects
	~ApplicationSolar();

	// react to key input
	void keyCallback(int key, int action, int mods);
	//handle delta mouse movement input
	void mouseCallback(double pos_x, double pos_y);
	//handle resizing
	void resizeCallback(unsigned width, unsigned height);
	Node planetColorGenerator(Node root);
	Node planetGenerator();
	// draw all objects
	void render() const;
	vec3 origin_position = { 0.0,0.0, 0.0, };
	
	// ASSIGNMENT 4 
	// making a function to initialize and load a cubemap for the skybox
	GLuint loadCubemap() const;
	GLuint skybox_vbo;
	GLuint skybox_vao;
	GLuint planet_texture0[8];
	vector<Node> children_list;
	Node root = Node();


	
	

protected:
	void initializeShaderPrograms();
	void initializeGeometry();
	void initializeStarsGeometry();
	// update uniform values
	void uploadUniforms();
	// upload projection matrix
	void uploadProjection();
	// upload view matrix
	void uploadView();


	// ASSIGNMENT 4 
	// making a function to initialize and load a cubemap for the skybox

	GLuint initializeTexturePrograms(pixel_data image, GLuint index) const;


	GLfloat* star_generator(GLfloat[]);

	// cpu representation of model
	model_object planet_object;
	// model stars
	model_object stars;

  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;

  // here we create a texture ID
 //const pixel_data image;
 const vector <Node> SolarSystem;
};

#endif