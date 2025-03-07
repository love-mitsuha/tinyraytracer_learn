#define STBIW_WINDOWS_UTF8
#define _USE_MATH_DEFINES
#define STB_IMAGE_IMPLEMENTATION

#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"
#include "Object.h"
#include "Light.h"
#include "Physics.h"
#include "stb_image.h"
#include "Background.h"
#include "model.h"
#include "Camera.h"
#include "raytracing.h"

Material      ivory(Vec3f(0.4, 0.4, 0.3), Vec4f(0.6, 0.3, 0.1, 0.0), 50., 1.0);
Material      glass(Vec3f(0.6, 0.7, 0.8), Vec4f(0.0, 0.5, 0.1, 0.8), 125., 1.5);
Material red_rubber(Vec3f(0.3, 0.1, 0.1), Vec4f(0.9, 0.1, 0.0, 0.0), 10., 1.0);
Material     mirror(Vec3f(1.0, 1.0, 1.0), Vec4f(0.0, 10.0, 0.8, 0.0), 1425., 1.0);

// 金属材质组
Material      gold(Vec3f(0.83f, 0.68f, 0.21f), Vec4f(0.3f, 0.8f, 0.1f, 0.0f), 150.0f, 1.0f);  // 黄金：高镜面反射
Material    silver(Vec3f(0.95f, 0.95f, 0.97f), Vec4f(0.2f, 0.9f, 0.1f, 0.0f), 200.0f, 1.0f);  // 白银：极高镜面反射
Material    copper(Vec3f(0.72f, 0.45f, 0.20f), Vec4f(0.4f, 0.6f, 0.2f, 0.0f), 100.0f, 1.0f);  // 铜：暖色调金属

// 宝石材质组
Material   diamond(Vec3f(0.82f, 0.82f, 0.90f), Vec4f(0.0f, 0.2f, 0.1f, 0.7f), 300.0f, 2.42f); // 钻石：高折射率
Material     jade(Vec3f(0.15f, 0.45f, 0.30f), Vec4f(0.5f, 0.1f, 0.2f, 0.3f), 50.0f, 1.6f);  // 翡翠：半透明材质
Material  sapphire(Vec3f(0.06f, 0.12f, 0.44f), Vec4f(0.1f, 0.3f, 0.1f, 0.5f), 180.0f, 1.77f); // 蓝宝石

// 液体材质组
Material      water(Vec3f(0.00f, 0.30f, 0.50f), Vec4f(0.0f, 0.1f, 0.1f, 0.8f), 150.0f, 1.33f); // 水：折射+散射
Material       oil(Vec3f(0.50f, 0.50f, 0.10f), Vec4f(0.1f, 0.2f, 0.1f, 0.6f), 80.0f, 1.47f); // 油性表面

// 塑料材质组
Material plastic_red(Vec3f(0.90f, 0.10f, 0.10f), Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 30.0f, 1.0f);  // 红色塑料
Material plastic_cyan(Vec3f(0.10f, 0.80f, 0.80f), Vec4f(0.8f, 0.2f, 0.0f, 0.0f), 40.0f, 1.0f); // 青色塑料

// 特殊材质组
Material    velvet(Vec3f(0.30f, 0.02f, 0.10f), Vec4f(0.9f, 0.0f, 0.0f, 0.0f), 5.0f, 1.0f);  // 天鹅绒：无反射
Material   ceramic(Vec3f(0.95f, 0.95f, 0.96f), Vec4f(0.7f, 0.3f, 0.2f, 0.0f), 80.0f, 1.0f);  // 陶瓷材质
Material  tungsten(Vec3f(0.35f, 0.33f, 0.30f), Vec4f(0.4f, 0.7f, 0.2f, 0.0f), 220.0f, 1.0f);  // 钨金属
Material    chrome(Vec3f(0.80f, 0.80f, 0.82f), Vec4f(0.3f, 0.9f, 0.3f, 0.0f), 250.0f, 1.0f);  // 镀铬表面

// 天然材质组
Material   marble(Vec3f(0.85f, 0.84f, 0.83f), Vec4f(0.6f, 0.3f, 0.2f, 0.0f), 60.0f, 1.0f);  // 大理石
Material    wood(Vec3f(0.40f, 0.25f, 0.15f), Vec4f(0.8f, 0.1f, 0.1f, 0.0f), 15.0f, 1.0f);  // 原木
Material  rubber(Vec3f(0.15f, 0.15f, 0.15f), Vec4f(0.9f, 0.0f, 0.0f, 0.0f), 8.0f, 1.0f);  // 黑色橡胶

const int width = 1024;
const int height = 768;
char* modelname = "D:\\tinyraytracer_learn\\duck.obj";
char* BGname = "D:\\tinyraytracer_learn\\envmap.jpg";

//检测是否碰撞并且修改参数
bool scene_intersect(const Vec3f& origin, const Vec3f& direction, std::vector<std::unique_ptr<Object>>& objects,Model &model,
    Vec3f& hit, Vec3f& normal, Material& material)
{
    float min_distance = std::numeric_limits<float>::max();
    caculate_objects(objects, min_distance, origin, direction, hit, normal, material);
    
    float model_dist;
    if (model.bbox_intersect(origin, direction))
    {
        //std::cerr << model.bbox_intersect(origin, direction);
        for (size_t i = 0; i < model.nfaces(); i++)
        {
            if (model.ray_triangle_intersect(i, origin, direction, model_dist) && model_dist < min_distance)
            {
                min_distance = model_dist;
                normal = model.get_face_normal(i);
                hit = origin + direction * model_dist;
                material = glass;
            }
        }
    }
    return min_distance < 1000;//如果大于10000则视为没有碰撞
}

//投射光线并返回颜色
Vec3f cast_ray(const Vec3f& origin, const Vec3f& direction, std::vector<std::unique_ptr<Object>>& spheres, Model& model,
    std::vector<Light>& lights,Background& background, size_t depth = 0)
{
    Vec3f hit, normal;//碰撞点 法线
    Material material;
    float intersect_dist = 0;
    if (depth > 4 || !scene_intersect(origin, direction, spheres, model, hit, normal, material))//递归深度超过4或没有碰撞则视为背景颜色
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
    Vec3f reflect_color = cast_ray(reflect_origin, reflect_dir, spheres,model, lights, background, depth + 1);
    //递归计算折射光
    Vec3f refract_dir = refract(direction, normal, material.refractive_cofficient).normalize();
    Vec3f refract_origin = refract_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//判断在外部还是内部
    Vec3f refract_color = cast_ray(refract_origin, refract_dir, spheres,model, lights, background, depth + 1);
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
        if (scene_intersect(shadow_origin, -light_dir, spheres,model, shadow_pt, shadow_normal, tmpmaterial) && 
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


void render(int width, int height, Camera& camera, std::vector<std::unique_ptr<Object>>& spheres, Model& model,
    std::vector<Light>& lights, std::vector<Vec3f>& framebuffer,Background& background)
{

    #pragma omp parallel for
    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            //相机在原点 朝-z方向 FOV为竖直方向
            //tan(FOV / 2) = (screen width) * 0.5 / (screen-camera distance) camera.z=-1
            //先转为ndc空间 (-1,1)
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(camera.FOV / 2) * (float)width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(camera.FOV / 2);//y轴进行翻转
            //Vec3f direction = Vec3f(x, y, -1).normalize();//光线投射方向 Vec3f(1,0,0)*x+Vec3f(0,1,0)*y+Vec3f(0,0,-1)*1
            Vec3f direction = (camera.forward * 1 + camera.right * x + camera.up * y).normalize();//相机坐标转世界坐标
            framebuffer[i + j * width] = cast_ray(camera.position, direction, spheres, model, lights, background, 0);
        }
    }
}

void image_write_ppm(int width, int height, int idx, std::vector<Vec3f>& framebuffer)
{
    std::ofstream ofs; // save the framebuffer to file
    ofs.open("..\\output\\output_"+std::to_string(idx)+".ppm", std::ofstream::out | std::ofstream::binary);//使用二进制模式
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i) {
        Vec3f &c = framebuffer[i];
        float color_max = std::max(c.x, std::max(c.y, c.z));
        if (color_max > 1.f) c = c * (1.f / color_max);//避免过曝
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
    std::cout << "picture" + std::to_string(idx) + " finished" << std::endl;
}


int main() 
{
    Camera camera(Vec3f(0., 0., -20.));
    Model duck(modelname);
    Background background(BGname); 

    std::vector<Vec3f> framebuffer(width * height);
    std::vector<std::unique_ptr<Object>> objects;
    std::vector<Light> lights;

    Plane plane1(Vec3f(0, -4, -20), 20, 20, chrome, mirror);
    Plane plane2(Vec3f(0, 4, -20), 20, 20, chrome, mirror);
    objects.push_back(std::make_unique<Plane>(plane1));
    objects.push_back(std::make_unique<Plane>(plane2));

    Sphere sphere1(Vec3f(-3, 0, -16), 2, gold);
    Sphere sphere2(Vec3f(-1.0, -1.5, -12), 2, glass);
    Sphere sphere3(Vec3f(1.5, -0.5, -18), 3, silver);
    Sphere sphere4(Vec3f(7, 5, -18), 4, mirror);
    objects.push_back(std::make_unique<Sphere>(sphere1));
    objects.push_back(std::make_unique<Sphere>(sphere2));
    objects.push_back(std::make_unique<Sphere>(sphere3));
    objects.push_back(std::make_unique<Sphere>(sphere4));

    Light light1 = Light(Vec3f(-20, 20, 20), 1.5);
    Light light2 = Light(Vec3f(30, 50, -25), 1.8);
    Light light3 = Light(Vec3f(30, 20, 30), 1.7);
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);

    render(width, height, camera, objects, duck, lights, framebuffer, background);
    image_write_ppm(width, height, 9999, framebuffer);


    /*for (size_t frameNum = 0; frameNum < 720; frameNum++)
    {
        camera.theta += (0.5 / 360.) * 2 * M_PI;
        
    }*/
    return 0;
}

