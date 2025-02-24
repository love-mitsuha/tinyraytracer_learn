#pragma once
#include "geometry.h"

Vec3f reflect(const Vec3f& in, const Vec3f& normal);

Vec3f refract(const Vec3f& in, const Vec3f& normal, const float refractive_coefficient);
