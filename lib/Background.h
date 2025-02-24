#pragma once
#include "geometry.h"


class Background
{
public:
	Background(){}
	Background(char* filename);
	~Background(){}

	unsigned char* data;
	char* filename;
	int width;
	int height;
	int channels;

	Vec3f get_color(int x, int y);
	
private:

};
