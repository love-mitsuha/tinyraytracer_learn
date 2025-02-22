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

	
	Vec3f diffuse_color;//������
	Vec4f albedo;//������
	float specular_cofficient;//�߹�ϵ��
	float refractive_cofficient;//����ϵ��
private:
	
};

