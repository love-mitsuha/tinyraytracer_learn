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
    return min_distance < 10000;//�������10000����Ϊû����ײ
}

//Ͷ����߲�������ɫ
Vec3f cast_ray(const Vec3f& origin, const Vec3f& direction, std::vector<Sphere>& spheres)
{
    Vec3f hit, normal;//��ײ�� ����
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
            //�����ԭ�� ��-z���� FOVΪ��ֱ����
            //tan(FOV / 2) = (screen width) * 0.5 / (screen-camera distance) camera.z=-1
            //��תΪndc�ռ� (-1,1)
            float x = (2 * (i + 0.5) / (float)width - 1) * tan(FOV / 2) * (float)width / (float)height;
            float y = -(2 * (j + 0.5) / (float)height - 1) * tan(FOV / 2);//y����з�ת
            Vec3f direction = Vec3f(x, y, -1).normalize();//����Ͷ�䷽��
            framebuffer[i + j * width] = cast_ray(Vec3f(0, 0, 0), direction, spheres);
        }
    }

}

void image_write_ppm(int width, int height, std::vector<Vec3f>& framebuffer)
{
    std::ofstream ofs; // save the framebuffer to file
    ofs.open("..\\out.ppm", std::ofstream::out | std::ofstream::binary);//ʹ�ö�����ģʽ
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
    Material      ivory(Vec3f(0.4, 0.4, 0.3));//����
    Material red_rubber(Vec3f(0.3, 0.1, 0.1));//��ɫ��

    // ��������
    Material gold(Vec3f(0.83, 0.68, 0.21));      // �ƽ�ɫ
    Material silver(Vec3f(0.72, 0.72, 0.72));   // ����ɫ
    Material copper(Vec3f(0.72, 0.45, 0.20));   // ��ͭɫ

    // ��ʯ����
    Material emerald(Vec3f(0.08, 0.82, 0.45));  // �̱�ʯ
    Material sapphire(Vec3f(0.06, 0.32, 0.73)); // ����ʯ
    Material amethyst(Vec3f(0.45, 0.12, 0.68)); // ��ˮ��

    // ����/�𽺲���
    Material plastic_blue(Vec3f(0.1, 0.2, 0.8));// ����ɫ����
    Material plastic_green(Vec3f(0.3, 0.9, 0.4));// ӫ��������

    // ��Ȼ����
    Material jade(Vec3f(0.2, 0.6, 0.4));        // �����
    Material sand(Vec3f(0.76, 0.70, 0.50));     // ɳ̲ɫ
    Material obsidian(Vec3f(0.06, 0.05, 0.08)); // ����ʯ

    // �Ƽ��в���
    Material cyber_cyan(Vec3f(0.3, 0.9, 0.9));  // ������
    Material neon_pink(Vec3f(1.0, 0.2, 0.6));   // �޺��
    Material hologram_blue(Vec3f(0.1, 0.4, 0.9));// ȫϢ��

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

