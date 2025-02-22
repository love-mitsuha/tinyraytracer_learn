#include "Object.h"
//返回是否相交同时修改最近的交点距离
bool Sphere::ray_intersect(const Vec3f& origin, const Vec3f& direction, float& distance)
{
	Vec3f link = center - origin;
	float tca = link * direction;
	float d2 = link * link - tca * tca;
	if (d2 > radius * radius) return false;
	float half = sqrtf(radius * radius - d2);
	distance = tca - half;
	if (distance < 0) distance = tca + half;//该方向上第一个交点在视线后方
	if (distance < 0) return false;//两个交点都在视线后方

	return true;
}








