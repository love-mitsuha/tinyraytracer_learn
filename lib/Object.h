#pragma once
#include "geometry.h"
#include "Material.h"

class Sphere
{
public:
	Sphere(){}
	Sphere(const Vec3f &c,float r):center(c),radius(r){}
	Sphere(const Vec3f &c,float r,const Material &m):center(c),radius(r),material(m){}
	~Sphere(){}
	bool ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance);

	Vec3f center;
	float radius;
	Material material;
private:
	
};

//¥π÷±y÷· (0,1,0)
class Plane
{
public:
	Plane(){}
	Plane(const Vec3f& center, const float width, const float height, const Material& material):
			center(center),width(width),height(height),material(material){ }
	Plane(const Vec3f& center, const float width, const float height):
		center(center), width(width), height(height) {}
	~Plane(){}

	bool ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance);

	Vec3f center;
	float width;
	float height;
	Material material;


private:

};




