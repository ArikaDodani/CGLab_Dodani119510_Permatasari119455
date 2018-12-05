#pragma once
#include <string>
#include <list>
#include <glm/glm.hpp>
#include <iostream>
#include "PointLightNode.h"
#include <vector>
#include <list>

using namespace std;
using namespace glm;

#ifndef NODE_H
#define NODE_H


class Node
{

public:
	Node();
	// the override constructor is collecting the planet properties
	Node(string, float, fvec3,float);
	~Node();
	Node getParent();
	void setParent(Node);
	Node getChildren(string);
	vector<Node> getChildrenList();
	string getName();
	string getPath();
	float getDepth(); // for now we are using depth to determine the scale. 
	mat4 getLocalTransform();
	void setLocalTransform(mat4);
	mat4 getWorldTransform();
	void setWorldTransform(mat4);
	void addChildren(Node);
	Node removeChildren(string);
	// this function returns the translation
	fvec3 getTranslation();
	// this function returns the rotation
	float getRotation();
	PointLightNode LightNode;
	vector<Node> children;
	vec3 planet_color;


private:
	Node *parent = {};

	string name;
	string path;
	float depth; // size of the planet is being stored here
	fvec3 Translate_values; // translation vector is being stored here 
	mat4 localTransform;
	mat4 worldTransform;
	float rotation_degree; //rotation degree is being stored here
};

#endif 
