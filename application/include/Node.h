#pragma once
#include <string>
#include <list>
#include <glm/glm.hpp>
#include <iostream>

using namespace std;
using namespace glm;

#ifndef NODE_H
#define NODE_H


class Node
{

public:
	Node();
	Node(string, float, fvec3);
	~Node();
	Node getParent();
	void setParent(Node);
	Node getChildren(string);
	list<Node> getChildrenList();
	string getName();
	string getPath();
	int getDepth();
	mat4 getLocalTransform();
	void setLocalTransform(mat4);
	mat4 getWorldTransform();
	void setWorldTransform(mat4);
	void addChildren(Node);
	Node removeChildren(string);
	fvec3 getTranslation();


private:
	Node *parent = {};
	list<Node> children;
	string name;
	string path;
	int depth;
	fvec3 Translate_values;
	mat4 localTransform;
	mat4 worldTransform;
};

#endif 
