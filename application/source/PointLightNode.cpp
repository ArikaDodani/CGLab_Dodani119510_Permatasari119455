#include "PointLightNode.h"



PointLightNode::PointLightNode()
{
}


PointLightNode::~PointLightNode(){

}

float PointLightNode::getLightIntensity() {
	return lightIntensity;

}
vec3 PointLightNode::getLightColor()
{
	return lightColor;
}

void PointLightNode::setLightIntensity(float lightIntensity2) {
	lightIntensity = lightIntensity2;
};

void PointLightNode::setLightColor(vec3 lightColor2) {
	lightColor = lightColor2;
};
