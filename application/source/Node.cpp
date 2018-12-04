#include "Node.h"


Node::Node()
{

}
// the override constructor is collecting the planet properties
Node::Node(string planet_name, float planet_size, fvec3 localTransform_translate, float rotation_d)
{


	name = planet_name;
	depth = planet_size;
	Translate_values = localTransform_translate;
	rotation_degree = rotation_d;



}


Node::~Node()
{
}


Node Node::getParent()
{
	return *parent;
}

void Node::setParent(Node parent_1) {
	// the windows was often giving a null pointer.
	try {
		parent = new Node();
		*parent = parent_1;
	}
	catch (...) {
		std::cout << "invalid user input" << endl;
	}

}
Node Node::getChildren(string) {
	Node temp;
	return temp;
}
list<Node> Node::getChildrenList() {
	return children;
}
string Node::getName() {
	return name;
}
string Node::getPath() {
	return path;
}
float Node::getDepth() {
	return depth;
}
mat4 Node::getLocalTransform() {
	return localTransform;
}

fvec3 Node::getTranslation() {
	return Translate_values;

}
void Node::setLocalTransform(mat4) {

}
mat4 Node::getWorldTransform() {
	return worldTransform;

}
void Node::setWorldTransform(mat4) {

}
void Node::addChildren(Node child) {
	children.push_back(child);

}
Node Node::removeChildren(string) {
	Node temp;
	return temp;

}

float Node::getRotation() {
	return rotation_degree;
	
}


