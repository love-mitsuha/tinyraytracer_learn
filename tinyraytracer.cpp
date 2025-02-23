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
#include "Light.h"
#include "stb_image_write.h"
#include "Physics.h"

Material      ivory(Vec3f(0.4, 0.4, 0.3), Vec4f(0.6, 0.3, 0.1, 0.0), 50., 1.0);
Material      glass(Vec3f(0.6, 0.7, 0.8), Vec4f(0.0, 0.5, 0.1, 0.8), 125., 1.5);
Material red_rubber(Vec3f(0.3, 0.1, 0.1), Vec4f(0.9, 0.1, 0.0, 0.0), 10., 1.0);
Material     mirror(Vec3f(1.0, 1.0, 1.0), Vec4f(0.0, 10.0, 0.8, 0.0), 1425., 1.0);

// ����������
Material      gold(Vec3f(0.83f, 0.68f, 0.21f), Vec4f(0.3f, 0.8f, 0.1f, 0.0f), 150.0f, 1.0f);  // �ƽ𣺸߾��淴��
Material    silver(Vec3f(0.95f, 0.95f, 0.97f), Vec4f(0.2f, 0.9f, 0.1f, 0.0f), 200.0f, 1.0f);  // ���������߾��淴��
Material    copper(Vec3f(0.72f, 0.45f, 0.20f), Vec4f(0.4f, 0.6f, 0.2f, 0.0f), 100.0f, 1.0f);  // ͭ��ůɫ������

// ��ʯ������
Material   diamond(Vec3f(0.82f, 0.82f, 0.90f), Vec4f(0.0f, 0.2f, 0.1f, 0.7f), 300.0f, 2.42f); // ��ʯ����������
Material     jade(Vec3f(0.15f, 0.45f, 0.30f), Vec4f(0.5f, 0.1f, 0.2f, 0.3f), 50.0f, 1.6f);  // ��䣺��͸������
Material  sapphire(Vec3f(0.06f, 0.12f, 0.44f), Vec4f(0.1f, 0.3f, 0.1f, 0.5f), 180.0f, 1.77f); // ����ʯ

// Һ�������
Material      water(Vec3f(0.00f, 0.30f, 0.50f), Vec4f(0.0f, 0.1f, 0.1f, 0.8f), 150.0f, 1.33f); // ˮ������+ɢ��
Material       oil(Vec3f(0.50f, 0.50f, 0.10f), Vec4f(0.1f, 0.2f, 0.1f, 0.6f), 80.0f, 1.47f); // ���Ա���

// ���ϲ�����
Material plastic_red(Vec3f(0.90f, 0.10f, 0.10f), Vec4f(0.9f, 0.1f, 0.0f, 0.0f), 30.0f, 1.0f);  // ��ɫ����
Material plastic_cyan(Vec3f(0.10f, 0.80f, 0.80f), Vec4f(0.8f, 0.2f, 0.0f, 0.0f), 40.0f, 1.0f); // ��ɫ����

// ���������
Material    velvet(Vec3f(0.30f, 0.02f, 0.10f), Vec4f(0.9f, 0.0f, 0.0f, 0.0f), 5.0f, 1.0f);  // ����ޣ��޷���
Material   ceramic(Vec3f(0.95f, 0.95f, 0.96f), Vec4f(0.7f, 0.3f, 0.2f, 0.0f), 80.0f, 1.0f);  // �մɲ���
Material  tungsten(Vec3f(0.35f, 0.33f, 0.30f), Vec4f(0.4f, 0.7f, 0.2f, 0.0f), 220.0f, 1.0f);  // �ٽ���
Material    chrome(Vec3f(0.80f, 0.80f, 0.82f), Vec4f(0.3f, 0.9f, 0.3f, 0.0f), 250.0f, 1.0f);  // �Ƹ�����

// ��Ȼ������
Material   marble(Vec3f(0.85f, 0.84f, 0.83f), Vec4f(0.6f, 0.3f, 0.2f, 0.0f), 60.0f, 1.0f);  // ����ʯ
Material    wood(Vec3f(0.40f, 0.25f, 0.15f), Vec4f(0.8f, 0.1f, 0.1f, 0.0f), 15.0f, 1.0f);  // ԭľ
Material  rubber(Vec3f(0.15f, 0.15f, 0.15f), Vec4f(0.9f, 0.0f, 0.0f, 0.0f), 8.0f, 1.0f);  // ��ɫ��

const int width = 1024;
const int height = 768;



//����Ƿ���ײ�����޸Ĳ���
bool scene_intersect(const Vec3f& origin, const Vec3f& direction, std::vector<Sphere>& spheres,
    Vec3f& hit, Vec3f& normal, Material& material)
{
    float min_distance = std::numeric_limits<float>::max();
    for (size_t i = 0; i < spheres.size(); i++)
    {
        float dist;
        if (spheres[i].ray_intersect(origin, direction, dist) && dist < min_distance)//��ײ����С����̾���
        {
            min_distance = dist;
            hit = origin + direction * dist;//������ײ��
            normal = (hit - spheres[i].center).normalize();//��ײ�㷨��
            material = spheres[i].material;
        }
    }
    
    Plane plane(Vec3f(0, -4, -20), 20, 20, chrome);
    float plane_dist;
    if (plane.ray_intersect(origin, direction, plane_dist) && plane_dist < min_distance)
    {
        min_distance = plane_dist;
        normal = Vec3f(0., 1., 0.);
        hit = origin + direction * plane_dist;
        material = (int(0.5 * hit.x + 1000) + int(0.5 * hit.z)) & 1 ? chrome : mirror;
    }
    return min_distance < 1000;//�������10000����Ϊû����ײ
}

//Ͷ����߲�������ɫ
Vec3f cast_ray(const Vec3f& origin, const Vec3f& direction, std::vector<Sphere>& spheres, std::vector<Light>& lights, size_t depth = 0)
{
    Vec3f hit, normal;//��ײ�� ����
    Material material;
    float intersect_dist = 0;
    if (depth > 4 || !scene_intersect(origin, direction, spheres, hit, normal, material))//�ݹ���ȳ���4��û����ײ����Ϊ������ɫ
    {
        return Vec3f(0.4, 0.3, 0.2);//background color
    }
    //�ݹ���㷴���
    Vec3f reflect_dir = reflect(direction, normal);
    Vec3f reflect_origin = reflect_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//�ж����ⲿ�����ڲ�
    Vec3f reflect_color = cast_ray(reflect_origin, reflect_dir, spheres, lights, depth + 1);
    //�ݹ���������
    Vec3f refract_dir = refract(direction, normal, material.refractive_cofficient).normalize();
    Vec3f refract_origin = refract_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//�ж����ⲿ�����ڲ�
    Vec3f refract_color = cast_ray(refract_origin, refract_dir, spheres, lights, depth + 1);
    //�������뾵�淴�� �����й�Դ���м���
    float diffuse = 0, specular = 0;
    for (size_t i = 0; i < lights.size(); i++)
    {
        Vec3f light_dir = (hit - lights[i].position).normalize();
        float light_distance = (hit - lights[i].position).norm();
        //��Ӱ
        Vec3f shadow_origin = -light_dir * normal > 0 ? hit + normal * 1e-3 : hit - normal * 1e-3;//������߷�������򽫵��������ڲ� �������м���
        Vec3f shadow_pt, shadow_normal;
        Material tmpmaterial;
        if (scene_intersect(shadow_origin, -light_dir, spheres, shadow_pt, shadow_normal, tmpmaterial) && (shadow_pt - shadow_origin).norm() < light_distance)
            continue;
        //������
        diffuse += std::max(-light_dir * normal, 0.f) * lights[i].intensity;
        //���淴��
        specular += powf(std::max(-reflect(light_dir, normal) * direction, 0.f), material.specular_cofficient) * lights[i].intensity;
    }
    return material.diffuse_color * diffuse * material.albedo.x + Vec3f(1., 1., 1.) * specular * material.albedo.y //��ɫ�߹�
        + reflect_color * material.albedo.z + refract_color * material.albedo.w;
}


void render(int width, int height, std::vector<Sphere>& spheres, std::vector<Light>& lights, std::vector<Vec3f>& framebuffer)
{
    const float FOV = MY_PI / 2;//vertical
    #pragma omp parallel for
    for (size_t j = 0; j < height; j++)
    {
        for (size_t i = 0; i < width; i++)
        {
            //�����ԭ�� ��-z���� FOVΪ��ֱ����
            //tan(FOV / 2) = (screen width) * 0.5 / (screen-camera distance) camera.z=-1
            //��תΪndc�ռ� (-1,1)
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(FOV / 2) * (float)width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(FOV / 2);//y����з�ת
            Vec3f direction = Vec3f(x, y, -1).normalize();//����Ͷ�䷽��
            framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), direction, spheres, lights);
        }
    }

}

void image_write_ppm(int width, int height, std::vector<Vec3f>& framebuffer)
{
    std::ofstream ofs; // save the framebuffer to file
    ofs.open("..\\out.ppm", std::ofstream::out | std::ofstream::binary);//ʹ�ö�����ģʽ
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i) {
        Vec3f &c = framebuffer[i];
        float color_max = std::max(c.x, std::max(c.y, c.z));
        if (color_max > 1.f) c = c * (1.f / color_max);//�������
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
}


int main() 
{
    
    
    std::vector<Vec3f> framebuffer(width * height);
    std::vector<Sphere> spheres;
    std::vector<Light> lights;

    Sphere sphere1(Vec3f(-3, 0, -16), 2, wood);
    Sphere sphere2(Vec3f(-1.0, -1.5, -12), 2, glass);
    Sphere sphere3(Vec3f(1.5, -0.5, -18), 3, silver);
    Sphere sphere4(Vec3f(7, 5, -18), 4, mirror);
    spheres.push_back(sphere1);
    spheres.push_back(sphere2);
    spheres.push_back(sphere3);
    spheres.push_back(sphere4);

    Light light1 = Light(Vec3f(-20, 20, 20), 1.5);
    Light light2 = Light(Vec3f(30, 50, -25), 1.8);
    Light light3 = Light(Vec3f(30, 20, 30), 1.7);
    lights.push_back(light1);
    lights.push_back(light2);
    lights.push_back(light3);

    render(width, height, spheres, lights, framebuffer);
    image_write_ppm(width, height, framebuffer);
    return 0;
}

