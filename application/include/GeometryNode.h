#pragma once
#include "Node.h"
#include <string>
#include <list>
#include <glm/glm.hpp>
#include "scenegraph.h"
#include "application_solar.hpp"
#include "window_handler.hpp"
#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;

#ifndef GEOMETRYNODE_H
#define GEOMETRYNODE_H




class GeometryNode
{
public:
	GeometryNode();
	~GeometryNode();
	model getGeometry(); 
	void setGeometry(model);

private:
	model geometry;
};

#endif