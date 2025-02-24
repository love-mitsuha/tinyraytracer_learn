#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include "Object.h"
//�����Ƿ��ཻ ͬʱ�޸�����Ľ������
bool Sphere::ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance)
{
	Vec3f link = center - origin;
	float tca = link * direction;
	float d2 = link * link - tca * tca;
	if (d2 > radius * radius) return false;//��Բ�ľ�����ڰ뾶 ��Բ��
	float half = sqrtf(radius * radius - d2);//������ҳ�
	distance = tca - half;
	if (distance < 0) distance = tca + half;//�÷����ϵ�һ�����������ߺ� ���ӵ���Բ��
	if (distance < 0) return false;//�������㶼�����ߺ�
	return true;
}

bool Plane::ray_intersect(const Vec3f& origin, const Vec3f& direction, float &distance)
{
	if (fabs(direction.y) < 1e-3) return false;//�ų�ƽ�й���
	float d = (center.y - origin.y) / direction.y;
	if (d < 0) return false;//�ų����潻��
	Vec3f hit = origin + direction * d;
	//��齻���Ƿ���ƽ����
	bool in_x = hit.x <= center.x + width / 2 && hit.x >= center.x - width / 2;
	bool in_z = hit.z >= center.z - height / 2 && hit.z <= center.z + height / 2;
	if (in_x && in_z)
	{
		distance = d;
		return true;
	}
	return false;
}

