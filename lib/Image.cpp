#include "Camera.h"
#include "Image.h"

Image::Image()
{
	width = 0;
	height = 0;
    framebuffer = std::vector<Vec3f>();
}

Image::Image(const int width, const int height)
	:width(width),height(height)
{
	framebuffer = std::vector<Vec3f>(width * height);
}

Image::~Image()
{
	
}

void Image::image_write_ppm(int idx)
{
    std::ofstream ofs; // save the framebuffer to file
    ofs.open("..\\output\\output_" + std::to_string(idx) + ".ppm", std::ofstream::out | std::ofstream::binary);//使用二进制模式
    ofs << "P6\n" << width << " " << height << "\n255\n";
    for (size_t i = 0; i < height * width; ++i) {
        Vec3f& c = framebuffer[i];
        float color_max = std::max(c.x, std::max(c.y, c.z));
        if (color_max > 1.f) c = c * (1.f / color_max);//避免过曝
        for (size_t j = 0; j < 3; j++) {
            ofs << (char)(255 * std::max(0.f, std::min(1.f, framebuffer[i][j])));
        }
    }
    ofs.close();
    std::cout << "picture" + std::to_string(idx) + " finished" << std::endl;
}

void Image::render(Camera& camera, std::vector<std::unique_ptr<Object>>& spheres, 
    std::vector<std::unique_ptr<Model>>& models, std::vector<Light>& lights, Background& background)
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
            //framebuffer[i + j * width] = cast_ray(camera.position, direction, spheres, models, lights, background, 0);
            framebuffer[i + j * width] = camera.cast_ray(camera.position, direction, spheres, models, lights, background, 0);
        }
    }
}
