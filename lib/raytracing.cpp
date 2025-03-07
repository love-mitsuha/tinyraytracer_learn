#include "raytracing.h"

void caculate_objects(std::vector<std::unique_ptr<Object>>& objects, float& min_distance, const Vec3f& origin,
	const Vec3f& direction, Vec3f& hit, Vec3f& normal, Material& material)
{
    for (size_t i = 0; i < objects.size(); i++)
    {
        float dist;
        if (objects[i].get()->ray_intersect(origin, direction, dist) && dist < min_distance)//碰撞并且小于最短距离
        {
            objects[i].get()->intersect_effective(*objects[i].get(), origin, direction, dist, min_distance, hit, normal, material);
        }
    }
}



