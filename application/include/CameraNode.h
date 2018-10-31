#pragma once
#include <string>
#include <list>
#include <glm/glm.hpp>
#include "Node.h"

using namespace std;
using namespace glm;

#ifndef CAMERANODE_H
#define CAMERANODE_H



class CameraNode
{
public:
	CameraNode();
	~CameraNode();
	bool getPerspective();
	bool getEnabled();
	void setEnabled(bool Enabled);
	mat4 getProjectionMatrix();
	mat4 setProjectionMatrix(mat4);

private:

	bool isPerspective = false;
	bool isEnabled = false;
	mat4 projectionMatrix;
};


#endif
