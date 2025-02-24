#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Background.h"
#include <stb_image.h>

Background::Background(char* filename) :filename(filename)
{
	data = stbi_load(filename, &width, &height, &channels, 0);
}


Vec3f Background::get_color(int x, int y)
{
	x = (x < 0) ? 0 : (x >= width) ? width - 1 : x;
	y = (y < 0) ? 0 : (y >= height) ? height - 1 : y;
	int offset = (x + (height - 1 - y) * width) * channels;
	unsigned char R = data[offset];
	unsigned char G = data[offset + 1];
	unsigned char B = data[offset + 2];
	unsigned char A = channels > 3 ? data[offset + 3] : 255;
	Vec3f color = Vec3f(R / 255., G / 255., B / 255.);
	return color;
}
