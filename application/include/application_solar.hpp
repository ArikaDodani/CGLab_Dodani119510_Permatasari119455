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


	// ASSIGNMENT 5
	void initializeFrameBuffer();
	void initializeQuadGeometry();
	void renderQuad() const;

	GLuint tex_handle;
	GLuint rb_handle;
	GLuint fbo_handle;

	// ASSIGNMENT 5 (ADDITIONAL TASK) 
	GLint effect = 0;
	GLint grayscale = 0;
	GLint blur = 0;


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
	void uploadPlanets(Node planet_display, int i) const;
	void uploadTexturedPlanets(Node planet_display, int i) const;
	void uploadSkyBox();

	// making a function to initialize and load a cubemap for the skybox
	void innitializeSkyBoxGeometry();
	GLuint initializeTexturePrograms(string filename, GLuint index) const;

	//ASSIGHMENT 2
	GLfloat* star_generator(GLfloat[]);
	// model stars
	model_object stars;


	// cpu representation of model
	model_object planet_object;


	//ASSIGNMENT 5
	// model object of the quad
	model_object quad_object;

  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;


};

#endif