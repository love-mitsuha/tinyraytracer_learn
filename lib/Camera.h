#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "geometry.h"
#include "Object.h"
#include "model.h"
#include "Light.h"
#include "Background.h"

class Camera
{
public:
	Camera(const Vec3f& target);
	~Camera();

	void set_position();
	void set_direction();
	friend void render(int width, int height, Camera& camera, std::vector<std::unique_ptr<Object>>& spheres, Model& model,
		std::vector<Light>& lights, std::vector<Vec3f>& framebuffer, Background& background);

private:
	Vec3f world_up = Vec3f(0.f, 1.f, 0.f);
	Vec3f forward;
	Vec3f right;
	Vec3f up;

	Vec3f position = Vec3f(0., 0., -1.);
	Vec3f target;
	float theta = 0;
	float phi = 0;
	float sight_distance;
	const float FOV = M_PI_2;
};

