#pragma once
#include "geometry.h"
#include "Material.h"

class Object
{
public:
	Object();
	Object(const Vec3f& center, const Material& material);
	~Object();

	virtual bool ray_intersect(const Vec3f& origin, const Vec3f& direction, float& distance) = 0;
	virtual void intersect_effective(const Object& object, const Vec3f& origin, const Vec3f& direction,
		float dist, float& min_dist, Vec3f& hit, Vec3f& normal, Material& material) = 0;

	Vec3f center;
	Material material;
protected:
	
	
};

class Sphere:public Object
{
public:
	Sphere();
	Sphere(const Vec3f& c, float r, const Material& m);
	~Sphere();
	bool ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance)override;
	void intersect_effective(const Object& object, const Vec3f& origin, const Vec3f& direction,
		float dist, float& min_dist, Vec3f& hit, Vec3f& normal, Material& material);

protected:
	float radius;
};

//¥π÷±y÷· (0,1,0)
class Plane:public Object
{
public:
	Plane();
	~Plane();
	Plane(const Vec3f& center, const float width, const float height);
	Plane(const Vec3f& center, const float width, const float height, const Material& material, const Material& extra_material);

	bool ray_intersect(const Vec3f& origin, const Vec3f& direction, float& distance)override;
	void intersect_effective(const Object& object, const Vec3f& origin, const Vec3f& direction,float dist, 
		float& min_dist, Vec3f& hit, Vec3f& normal, Material& material);
protected:
	float width;
	float height;
	Material extra_material;
};




