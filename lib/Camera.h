#pragma once
#define _USE_MATH_DEFINES
#include <cmath>
#include "geometry.h"
#include "Object.h"
#include "model.h"
#include "Light.h"
#include "Background.h"
#include "raytracing.h"
#include "Image.h"
class Camera
{
public:
	Camera(const Vec3f& target);
	~Camera();

	void set_position();
	void set_direction();

	Vec3f cast_ray(const Vec3f& origin, const Vec3f& direction, std::vector<std::unique_ptr<Object>>& objects,
		std::vector<std::unique_ptr<Model>>& models,
		std::vector<Light>& lights, Background& background, size_t depth = 0);
	friend void Image::render(Camera& camera, std::vector<std::unique_ptr<Object>>& spheres,
		std::vector<std::unique_ptr<Model>>& models, std::vector<Light>& lights, Background& background);

	


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

