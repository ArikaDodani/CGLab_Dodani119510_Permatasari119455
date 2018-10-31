#pragma once

#include <string>
#include "Node.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace std;
using namespace glm;
 
#ifndef SCENEGRAPH_H
#define SCENEGRAPH_H


class scenegraph
{
public:
	// Default constructor
	scenegraph();
	~scenegraph();
	string getName();
	Node getRoot();
	string printGraph(string);


private:
	// Member variables
	string name;
	Node Root_1;
	void setName(string);
	void setRoot(Node);


};

#endif
