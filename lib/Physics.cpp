#include "Physics.h"

Vec3f reflect(const Vec3f& in, const Vec3f& normal)
{
    return in - normal * (in * normal) * 2.;
}

Vec3f refract(const Vec3f& in, const Vec3f& normal, const float refractive_cofficient)
{
    float cosi = std::max(-1.f, std::min(1.f, -in * normal));
    float refractive_index_outer = 1, refractive_index_inner = refractive_cofficient;//Ĭ�ϴ��ⲿ�����ڲ�
    Vec3f n = normal;
    if (cosi < 0)//������ڲ������ⲿ ת��������������һ����ģʽ
    {
        n = -n;
        std::swap(refractive_index_outer, refractive_index_inner);
        cosi = -cosi;
    }
    float refractive_ratio = refractive_index_outer / refractive_index_inner;
    float k = 1 - refractive_ratio * refractive_ratio * (1 - cosi * cosi);
    //���kС���� ˵�������sinֵ����1 ����ȫ����
    return k > 0 ? in * refractive_ratio + n * (refractive_ratio * cosi - sqrt(k)) : Vec3f(0., 0., 0.);
}