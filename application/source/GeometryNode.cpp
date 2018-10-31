#include "GeometryNode.h"



GeometryNode::GeometryNode()
{
}

GeometryNode::~GeometryNode()
{
}


model GeometryNode::getGeometry() {
	return geometry;
}
void GeometryNode::setGeometry(model geometry1) {
	geometry = geometry1;
}