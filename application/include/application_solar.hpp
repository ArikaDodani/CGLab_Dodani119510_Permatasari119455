#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include <tuple>
#include <list>
#include <glm/glm.hpp>
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

	// draw all objects
	void render() const;
	// create the data structure of star values
	

protected:
	void initializeShaderPrograms();
	void initializeGeometry();
	// update uniform values
	void uploadUniforms();
	// upload projection matrix
	void uploadProjection();
	// upload view matrix
	void uploadView();
	GLfloat* star_generator(GLfloat[]);

	// cpu representation of model
	model_object planet_object;
	// model stars
	model_object stars;

  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
};

#endif