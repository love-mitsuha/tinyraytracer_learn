#pragma once
#include "geometry.h"
#include "Object.h"
#include "Material.h"
#include "Physics.h"
#include "model.h"

void caculate_objects(std::vector<std::unique_ptr<Object>>& spheres, float& min_distance, 
	const Vec3f& origin, const Vec3f& direction,Vec3f& hit, Vec3f& normal, Material& material);

void caculate_models(std::vector<std::unique_ptr<Model>>& models, float& min_distance,
	const Vec3f& origin, const Vec3f& direction, Vec3f& hit, Vec3f& normal, Material& material);

bool scene_intersect(const Vec3f& origin, const Vec3f& direction, std::vector<std::unique_ptr<Object>>& objects, 
	std::vector<std::unique_ptr<Model>>& models,Vec3f& hit, Vec3f& normal, Material& material);

