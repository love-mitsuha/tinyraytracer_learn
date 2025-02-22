#pragma once
#include "geometry.h"
class Material
{
public:
	Material(){}
	~Material(){}
	Material(Vec3f diffuse_color):diffuse_color(diffuse_color){}
	
	Vec3f diffuse_color;

private:
	
};

