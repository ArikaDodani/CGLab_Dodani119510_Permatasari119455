#pragma once
#pragma once
#include <string>
#include <list>
#include <glm/glm.hpp>
#include <iostream>


using namespace std;
using namespace glm;

#ifndef POINTLIGHTNODE_H
#define POINTLIGHTNODE_H


class PointLightNode
{

public:
	PointLightNode();
	~PointLightNode();
	float lightIntensity;
	vec3 lightColor;
	void setLightIntensity(float lightIntensity2);
	void setLightColor(vec3 lightColor2);
	float getLightIntensity();
	vec3 getLightColor();


};

#endif 