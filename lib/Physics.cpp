#include "Physics.h"

Vec3f reflect(const Vec3f& in, const Vec3f& normal)
{
    return in - normal * (in * normal) * 2.;
}

Vec3f refract(const Vec3f& in, const Vec3f& normal, const float refractive_cofficient)
{
    float cosi = std::max(-1.f, std::min(1.f, -in * normal));
    float refractive_index_outer = 1, refractive_index_inner = refractive_cofficient;//默认从外部射向内部
    Vec3f n = normal;
    if (cosi < 0)//如果从内部射向外部 转换成与由外向内一样的模式
    {
        n = -n;
        std::swap(refractive_index_outer, refractive_index_inner);
        cosi = -cosi;
    }
    float refractive_ratio = refractive_index_outer / refractive_index_inner;
    float k = 1 - refractive_ratio * refractive_ratio * (1 - cosi * cosi);
    //如果k小于零 说明折射角sin值大于1 发生全反射
    return k > 0 ? in * refractive_ratio + n * (refractive_ratio * cosi - sqrt(k)) : Vec3f(0., 0., 0.);
}