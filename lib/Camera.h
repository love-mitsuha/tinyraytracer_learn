#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "geometry.h"

class Camera
{
public:
	Camera(){}
	Camera(const Vec3f& target) :target(target) {
		sight_distance = fabsf((target - position).norm());
	}


	~Camera(){}

	Vec3f world_up = Vec3f(0.f, 1.f, 0.f);
	Vec3f forward;
	Vec3f right;
	Vec3f up;

	Vec3f position = Vec3f(0., 0., 0.);
	Vec3f target;
	float theta = 0;
	float phi = 0;
	float sight_distance;
	const float FOV = M_PI_2;

	void set_position();
	void set_direction();


private:

};

