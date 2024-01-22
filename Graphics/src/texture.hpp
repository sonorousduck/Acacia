#pragma once

#include <glad/glad.h>
#include <cstdint>
#include <glm/glm.hpp>

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
	bool isAtlas;
	int imageCountX = 1, imageCountY = 1;
	std::uint64_t layer = 0;
	glm::vec2 Offset{0.0f};
	std::uint16_t currentImage = 0;

	Texture2D();
	Texture2D(unsigned int ID);
	Texture2D(unsigned int ID, std::uint64_t layer);

	void Generate(int width, int height, char* data);

	void Generate3D(int width, int height, char* data, std::uint16_t tilesX, std::uint16_t tilesY);

	void Bind() const;

	glm::vec2 getDimensions() { return glm::vec2(Width, Height); }
};