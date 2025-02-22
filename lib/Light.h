#pragma once
#include "geometry.h"

class Light
{
public:
	Light(){}
	~Light(){}
	Light(const Vec3f& position,float intensity):position(position),intensity(intensity){}

	Vec3f position;
	float intensity;

private:



};
