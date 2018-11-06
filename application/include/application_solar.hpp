#ifndef APPLICATION_SOLAR_HPP
#define APPLICATION_SOLAR_HPP

#include "application.hpp"
#include "model.hpp"
#include "structs.hpp"
#include <list>

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

protected:
	void initializeShaderPrograms();
	void initializeGeometry();
	// update uniform values
	void uploadUniforms();
	// upload projection matrix
	void uploadProjection();
	// upload view matrix
	void uploadView();

	// cpu representation of model
	model_object planet_object;

	// model stars
	model_object stars;

	// empty float values initialized
	float star_parameters[2][3] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };
	//fvec3 star_parameter[2] = { {0.0,0.0,0.0},{0.0,0.0,0.0} };
	//list<fvec3> star_size;
  
  // camera transform matrix
  glm::fmat4 m_view_transform;
  // camera projection matrix
  glm::fmat4 m_view_projection;
};

#endif