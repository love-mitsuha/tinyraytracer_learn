#include "Object.h"
//�����Ƿ��ཻͬʱ�޸�����Ľ������
bool Sphere::ray_intersect(const Vec3f& origin, const Vec3f& direction, float& distance)
{
	Vec3f link = center - origin;
	float tca = link * direction;
	float d2 = link * link - tca * tca;
	if (d2 > radius * radius) return false;
	float half = sqrtf(radius * radius - d2);
	distance = tca - half;
	if (distance < 0) distance = tca + half;//�÷����ϵ�һ�����������ߺ�
	if (distance < 0) return false;//�������㶼�����ߺ�

	return true;
}








