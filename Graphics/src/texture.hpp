#pragma once

#include <glad/glad.h>

class Texture2D
{
public:
	unsigned int ID;
	unsigned int Width, Height;
	unsigned int Internal_Format;
	unsigned int Image_Format;

	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Filter_Min;
	unsigned int Filter_Max;

	Texture2D();
	Texture2D(unsigned int ID);

	void Generate(unsigned int width, unsigned int height, char* data);

	void Bind() const;
};