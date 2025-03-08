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
#include "Image.h"

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

int main() 
{
    //照片 相机 背景图初始化
    Image image(width, height);
    Camera camera(Vec3f(0., 0., -20.));
    Background background(BGname); 

    //准备物体
    std::vector<std::unique_ptr<Object>> objects;
    Plane plane1(Vec3f(0, -4, -20), 20, 20, chrome, mirror);
    Plane plane2(Vec3f(0, 4, -20), 20, 20, chrome, mirror);
    Sphere sphere1(Vec3f(-3, 0, -16), 2, gold);
    Sphere sphere2(Vec3f(-1.0, -1.5, -12), 2, glass);
    Sphere sphere3(Vec3f(1.5, -0.5, -18), 3, silver);
    Sphere sphere4(Vec3f(7, 5, -18), 4, mirror);
    objects.push_back(std::make_unique<Plane>(plane1));
    objects.push_back(std::make_unique<Plane>(plane2));
    objects.push_back(std::make_unique<Sphere>(sphere1));
    objects.push_back(std::make_unique<Sphere>(sphere2));
    objects.push_back(std::make_unique<Sphere>(sphere3));
    objects.push_back(std::make_unique<Sphere>(sphere4));

    //准备光源
    std::vector<Light> lights;
    Light light1 = Light(Vec3f(-20, 20, 20), 1.5);
    Light light2 = Light(Vec3f(30, 50, -25), 1.8);
    Light light3 = Light(Vec3f(30, 20, 30), 1.7);
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);

    //准备模型
    std::vector<std::unique_ptr<Model>> models;
    Model duck(modelname, glass);
    models.push_back(std::make_unique<Model>(duck));

    //渲染并输出
    image.render(camera, objects, models, lights, background);
    image.image_write_ppm(9999);

    /*for (size_t frameNum = 0; frameNum < 720; frameNum++)
    {
        camera.theta += (0.5 / 360.) * 2 * M_PI;
        
    }*/
    return 0;
}

