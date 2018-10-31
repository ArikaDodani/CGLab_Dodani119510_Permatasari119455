#include "Node.h"

Node::Node()
{

}

Node::Node(string planet_name, float planet_size, fvec3 localTransform_translate)
{


	name = planet_name;
	depth = planet_size;
	Translate_values = localTransform_translate;



}


Node::~Node()
{
}


Node Node::getParent()
{
	return *parent;
}

void Node::setParent(Node parent_1) {

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
int Node::getDepth() {
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


