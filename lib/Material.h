#pragma once
#include "geometry.h"
class Material
{
public:
	Material(){}
	~Material(){}
	Material(Vec3f diffuse_color):diffuse_color(diffuse_color){}
	Material(const Vec3f& diffuse_color, const Vec4f& albedo, float specular_cofficient,float refractive_cofficient):
			diffuse_color(diffuse_color),albedo(albedo),specular_cofficient(specular_cofficient), 
			refractive_cofficient(refractive_cofficient){}

	
	Vec3f diffuse_color;//漫反射
	Vec4f albedo;//反照率
	float specular_cofficient;//高光系数
	float refractive_cofficient;//折射系数
private:
	
};

