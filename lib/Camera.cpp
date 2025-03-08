#include "Camera.h"

Camera::Camera(const Vec3f& target) :target(target)
{
	sight_distance = fabsf((target - position).norm());
	set_direction();
	set_position();
}

Camera::~Camera()
{
}

void Camera::set_position()
{
	position.x = sight_distance * cosf(phi) * sinf(theta) + target.x;
	position.y = sight_distance * sinf(phi) + target.y;
	position.z = sight_distance * cosf(phi) * cosf(theta) + target.z;
}

void Camera::set_direction()
{
	forward = (target - position).normalize();
	right = cross(forward, world_up).normalize();
	up = cross(right, forward);
}
//投射光线并返回颜色
Vec3f Camera::cast_ray(const Vec3f& origin, const Vec3f& direction, std::vector<std::unique_ptr<Object>>& objects, 
	std::vector<std::unique_ptr<Model>>& models, std::vector<Light>& lights, Background& background, size_t depth)
{
    Vec3f hit, normal;//碰撞点 法线
    Material material;
    float intersect_dist = 0;
    if (depth > 4 || !scene_intersect(origin, direction, objects, models, hit, normal, material))//递归深度超过4或没有碰撞则视为背景颜色
    {
        float phi = asinf(direction.y);
        float theta = atan2f(direction.x, -direction.z);//照片360° 相机指向左侧1/4处
        int x = static_cast<int>((theta + M_PI) / (2 * M_PI) * background.width);
        int y = static_cast<int>((phi + M_PI_2) / M_PI * background.height);
        return background.get_color(x, y);
    }
    //递归计算反射光
    Vec3f reflect_dir = reflect(direction, normal);
    Vec3f reflect_origin = reflect_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//判断在外部还是内部
    Vec3f reflect_color = cast_ray(reflect_origin, reflect_dir, objects, models, lights, background, depth + 1);
    //递归计算折射光
    Vec3f refract_dir = refract(direction, normal, material.refractive_cofficient).normalize();
    Vec3f refract_origin = refract_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//判断在外部还是内部
    Vec3f refract_color = cast_ray(refract_origin, refract_dir, objects, models, lights, background, depth + 1);
    //漫反射与镜面反射 和所有光源进行计算
    float diffuse = 0, specular = 0;
    for (size_t i = 0; i < lights.size(); i++)
    {
        Vec3f light_dir = (hit - lights[i].position).normalize();
        float light_distance = (hit - lights[i].position).norm();
        //阴影
        Vec3f shadow_origin = -light_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//如果光线方向错误则将点移至球内部 即不进行计算
        Vec3f shadow_pt, shadow_normal;
        Material tmpmaterial;
        if (scene_intersect(shadow_origin, -light_dir, objects, models, shadow_pt, shadow_normal, tmpmaterial) &&
            (shadow_pt - shadow_origin).norm() < light_distance)
            continue;
        //漫反射
        diffuse += std::max(-light_dir * normal, 0.f) * lights[i].intensity;
        //镜面反射
        specular += powf(std::max(-reflect(light_dir, normal) * direction, 0.f), material.specular_cofficient) * lights[i].intensity;
    }
    return material.diffuse_color * diffuse * material.albedo.x + material.diffuse_color * specular * material.albedo.y //白色高光
        + reflect_color * material.albedo.z + refract_color * material.albedo.w;
}
