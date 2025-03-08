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

void caculate_models(std::vector<std::unique_ptr<Model>>& models, float& min_distance, const Vec3f& origin, 
    const Vec3f& direction, Vec3f& hit, Vec3f& normal, Material& material)
{
    for (int i = 0; i < models.size(); i++)
    {
        if (models[i].get()->bbox_intersect(origin, direction))
        {
            
            float model_dist;
            //std::cerr << model.bbox_intersect(origin, direction);
            for (size_t j = 0; j < models[i].get()->nfaces(); j++)
            {
                if (models[i].get()->ray_triangle_intersect(j, origin, direction, model_dist) && model_dist < min_distance)
                {
                    min_distance = model_dist;
                    normal = models[i].get()->get_face_normal(j);
                    hit = origin + direction * model_dist;
                    material = models[i].get()->material;
                }
            }
        }
    }
}
//检测是否碰撞并且修改参数
bool scene_intersect(const Vec3f& origin, const Vec3f& direction, std::vector<std::unique_ptr<Object>>& objects,
    std::vector<std::unique_ptr<Model>>& models, Vec3f& hit, Vec3f& normal, Material& material)
{
    float min_distance = std::numeric_limits<float>::max();
    caculate_objects(objects, min_distance, origin, direction, hit, normal, material);
    caculate_models(models, min_distance, origin, direction, hit, normal, material);
    return min_distance < 1000;//如果大于10000则视为没有碰撞
}

