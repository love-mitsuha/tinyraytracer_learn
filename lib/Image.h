#pragma once
#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include "geometry.h"
#include "Object.h"
#include "Light.h"
#include "Background.h"
#include "model.h"
#include "Camera.h"
class Camera;
class Image
{
public:
	Image();
	Image(const int width, const int height);
	~Image();

	void image_write_ppm(int idx);
	void render(Camera& camera, std::vector<std::unique_ptr<Object>>& spheres,
		std::vector<std::unique_ptr<Model>>& models, std::vector<Light>& lights,Background& background);
private:
	int width;
	int height;
	std::vector<Vec3f> framebuffer;


};



