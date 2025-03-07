#pragma once
#include "geometry.h"
#include "Object.h"
#include "Material.h"

void caculate_objects(std::vector<std::unique_ptr<Object>>& spheres, float& min_distance, const Vec3f& origin, const Vec3f& direction,
	Vec3f& hit, Vec3f& normal, Material& material);
