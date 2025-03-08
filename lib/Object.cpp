#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Object.h"

//返回是否相交 同时修改最近的交点距离
bool Sphere::ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance)
{
	Vec3f link = center - origin;
	float tca = link * direction;
	float d2 = link * link - tca * tca;
	if (d2 > radius * radius) return false;//到圆心距离大于半径 在圆外
	float half = sqrtf(radius * radius - d2);//计算半弦长
	distance = tca - half;
	if (distance < 0) distance = tca + half;//该方向上第一个交点在视线后方 即视点在圆内
	if (distance < 0) return false;//两个交点都在视线后方
	return true;
}

bool Plane::ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance)
{
	if (fabs(direction.y) < 1e-3) return false;//排除平行光线
	float d = (center.y - origin.y) / direction.y;
	if (d < 0) return false;//排除背面交点
	Vec3f hit = origin + direction * d;
	//检查交点是否在平面上
	bool in_x = hit.x <= center.x + width / 2 && hit.x >= center.x - width / 2;
	bool in_z = hit.z >= center.z - height / 2 && hit.z <= center.z + height / 2;
	if (in_x && in_z)
	{
		distance = d;
		return true;
	}
	return false;
}

Object::Object(const Vec3f& center, const Material& material) :center(center), material(material) {}
Object::Object() :center(Vec3f()), material(Material()) {}
Object::~Object(){}

Sphere::Sphere() :Object(), radius(0.f) {}
Sphere::Sphere(const Vec3f& c, float r, const Material& m) :Object(c, m), radius(r) {}
Sphere::~Sphere(){}
void Sphere::intersect_effective(const Object& object, const Vec3f& origin, const Vec3f& direction, float dist, float& min_dist, Vec3f& hit, Vec3f& normal, Material& material)
{
	min_dist = dist;
	hit = origin + direction * dist;
	normal = (hit - object.center).normalize();
	material = object.material;
}


Plane::Plane() :Object(), width(0.), height(0.),extra_material(Material()){}
Plane::~Plane() {};
Plane::Plane(const Vec3f& center, const float width, const float height, const Material& material,const Material& extra_material):
	Object(center, material), width(width), height(height),extra_material(extra_material) {}
Plane::Plane(const Vec3f& center, const float width, const float height) :
	Object(center,Material()), width(width), height(height) { }
void Plane::intersect_effective(const Object& object, const Vec3f& origin, const Vec3f& direction, float dist, float& min_dist,
		Vec3f& hit, Vec3f& normal, Material& material)
{
	min_dist = dist;
	hit = origin + direction * dist;
	normal = Vec3f(0., 1., 0.);
	material = (int(0.5 * hit.x + 1000) + int(0.5 * hit.z)) & 1 ? object.material : extra_material;
}