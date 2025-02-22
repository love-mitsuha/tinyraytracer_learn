#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STBIW_WINDOWS_UTF8
#define MY_PI 3.1416

#include <limits>
#include <cmath>
#include <iostream>
#include <fstream>
#include <vector>
#include "geometry.h"
#include "Object.h"
#include "stb_image_write.h"

//检测是否碰撞并且修改参数
bool scene_intersect(const Vec3f& origin, const Vec3f& direction, std::vector<Sphere>& spheres,
    Vec3f& hit, Vec3f& normal, Material& material)
{
    float min_distance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); i++)
    {
        float dist;
        if (spheres[i].ray_intersect(origin, direction, dist) && dist < min_distance)//碰撞并且小于最短距离
        {
            min_distance = dist;
            hit = origin + direction * dist;//计算碰撞点
            normal = (hit - spheres[i].center).normalize();//碰撞点法线
            material = spheres[i].material;
        }
    }
    return min_distance < 10000;//如果大于10000则视为没有碰撞
}

//投射光线并返回颜色
Vec3f cast_ray(const Vec3f& origin, const Vec3f& direction, std::vector<Sphere>& spheres)
{
    Vec3f hit, normal;//碰撞点 法线
    Material material;
    float intersect_dist = 0;
    if (!scene_intersect(origin, direction, spheres, hit, normal, material))
    {
        return Vec3f(0., 1., 1.);//background color
    }
    return material.diffuse_color;//Sphere color
}



void render(int width, int height, std::vector<Sphere>& spheres, std::vector<Vec3f>& framebuffer)
{
    const float FOV = MY_PI / 2;//vertical
    #pragma omp parallel for
    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            //相机在原点 朝-z方向 FOV为竖直方向
            //tan(FOV / 2) = (screen width) * 0.5 / (screen-camera distance) camera.z=-1
            //先转为ndc空间 (-1,1)
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(FOV / 2) * (float)width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(FOV / 2);//y轴进行翻转
            Vec3f direction = Vec3f(x, y, -1).normalize();//光线投射方向
            framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), direction, spheres);
        }
    }

}

void image_write_ppm(int width, int height, std::vector<Vec3f>& framebuffer)
{
    std::ofstream ofs; // save the framebuffer to file
    ofs.open("..\\out.ppm", std::ofstream::out | std::ofstream::binary);//使用二进制模式
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i) {
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}


int main() 
{
    Material      ivory(Vec3f(0.4, 0.4, 0.3));//象牙
    Material red_rubber(Vec3f(0.3, 0.1, 0.1));//红色橡胶

    // 金属材质
    Material gold(Vec3f(0.83, 0.68, 0.21));      // 黄金色
    Material silver(Vec3f(0.72, 0.72, 0.72));   // 银白色
    Material copper(Vec3f(0.72, 0.45, 0.20));   // 古铜色

    // 宝石材质
    Material emerald(Vec3f(0.08, 0.82, 0.45));  // 绿宝石
    Material sapphire(Vec3f(0.06, 0.32, 0.73)); // 蓝宝石
    Material amethyst(Vec3f(0.45, 0.12, 0.68)); // 紫水晶

    // 塑料/橡胶材质
    Material plastic_blue(Vec3f(0.1, 0.2, 0.8));// 亮蓝色塑料
    Material plastic_green(Vec3f(0.3, 0.9, 0.4));// 荧光绿塑料

    // 自然材质
    Material jade(Vec3f(0.2, 0.6, 0.4));        // 翡翠绿
    Material sand(Vec3f(0.76, 0.70, 0.50));     // 沙滩色
    Material obsidian(Vec3f(0.06, 0.05, 0.08)); // 黑曜石

    // 科技感材质
    Material cyber_cyan(Vec3f(0.3, 0.9, 0.9));  // 赛博青
    Material neon_pink(Vec3f(1.0, 0.2, 0.6));   // 霓虹粉
    Material hologram_blue(Vec3f(0.1, 0.4, 0.9));// 全息蓝

    const int width = 1024;
    const int height = 768;
    
    std::vector<Vec3f> framebuffer(width * height);
    std::vector<Sphere> spheres;
    Sphere sphere1(Vec3f(-4, 0, -16),4, ivory);
    Sphere sphere2(Vec3f(4, 1, -8), 3, red_rubber);
    Sphere sphere3(Vec3f(-2, 6, -10), 2, copper);
    Sphere sphere4(Vec3f(1, -3, -12), 4, amethyst);
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);
    render(width, height, spheres, framebuffer);
    image_write_ppm(width, height, framebuffer);

    return 0;
}

