#include "scenegraph.h"
#include "application_solar.hpp"
#include "window_handler.hpp"
#include "utils.hpp"
#include "shader_loader.hpp"
#include "model_loader.hpp"
#include "Node.h"
#include <string>

#include <glbinding/gl/gl.h>
// use gl definitions from glbinding 

//dont load gl bindings from glfw
#define GLFW_INCLUDE_NONE
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>


scenegraph::scenegraph() {
}



scenegraph::~scenegraph()
{
}

string scenegraph::getName() {
	return name;
}
Node scenegraph::getRoot() {
	return Root_1;
}

void scenegraph::setName(string namee) {
	name = namee;

}
void scenegraph::setRoot(Node roott) {
	Root_1 = roott;
};

string scenegraph::printGraph(string dir) {
	string x = "temp";
	return x;
}