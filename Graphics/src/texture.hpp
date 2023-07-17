#pragma once

#include <glad/glad.h>
#include <cstdint>

class Texture2D
{
public:
	unsigned int ID;
	int Width, Height;
	unsigned int Internal_Format;
	unsigned int Image_Format;

	unsigned int Wrap_S;
	unsigned int Wrap_T;
	unsigned int Filter_Min;
	unsigned int Filter_Max;
	bool isArray;

	Texture2D();
	Texture2D(unsigned int ID);

	void Generate(int width, int height, char* data);
	void Generate3D(int width, int height, char* data, std::uint16_t tilesX, std::uint16_t tilesY);

	void Bind() const;
};