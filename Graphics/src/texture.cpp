#include "texture.hpp"
#include <algorithm>
#include <vector>
#include <iostream>

Texture2D::Texture2D()
	: Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_NEAREST), isArray(false), isAtlas(false)
{
	glGenTextures(1, &this->ID);
}

Texture2D::Texture2D(unsigned int ID)
	: ID(ID), Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_NEAREST), isArray(false), isAtlas(false)
{
}


Texture2D::Texture2D(unsigned int ID, std::uint64_t layer)
	: ID(ID), Width(0), Height(0), Internal_Format(GL_RGB), Image_Format(GL_RGB), Wrap_S(GL_CLAMP_TO_EDGE), Wrap_T(GL_CLAMP_TO_EDGE), Filter_Min(GL_LINEAR), Filter_Max(GL_NEAREST), isArray(false), isAtlas(false), layer(layer)
{
}


void Texture2D::Generate(int width, int height, char* data)
{
	this->Width = width;
	this->Height = height;
	this->isArray = false;



	// Create texture
	glBindTexture(GL_TEXTURE_2D, this->ID);
	glTexImage2D(GL_TEXTURE_2D, 0, this->Internal_Format, width, height, 0, this->Image_Format, GL_UNSIGNED_BYTE, data);

	// Set textre wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D, 0);
}

void Texture2D::Generate3D(int width, int height, char* data, std::uint16_t tilesX, std::uint16_t tilesY)
{
	this->Width = width;
	this->Height = height;
	this->isArray = true;

	int tileWidth = width / tilesX; 
	int tileHeight = height / tilesY;
	int channels = 4;
	int imageCount = tilesX * tilesY;

	glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);

	//glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format,
	//	tileWidth, tileHeight, imageCount, 0,
	//	this->Image_Format, GL_UNSIGNED_BYTE, data);



	// Create the image
	glTexImage3D(GL_TEXTURE_2D_ARRAY, 0, this->Internal_Format,
		tileWidth, tileHeight, imageCount, 0,
		this->Image_Format, GL_UNSIGNED_BYTE, nullptr);

	std::vector<char> tile(tileWidth * tileHeight * channels);
	int tileSizeX = tileWidth * channels;
	int rowLen = tilesX * tileSizeX;

	for (int iy = 0; iy < tilesY; ++iy)
	{
		for (int ix = 0; ix < tilesX; ++ix)
		{
			char* ptr = data + iy * rowLen * tileHeight + ix * tileSizeX;
			for (int row = 0; row < tileHeight; ++row)
				std::copy(ptr + row * rowLen, ptr + row * rowLen + tileSizeX,
					tile.begin() + (static_cast<std::_Vector_iterator<std::vector<char, std::allocator<char>>>::difference_type>(row) * tileSizeX));


			int i = iy * tilesX + ix;
			glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0,
				0, 0, i,
				tileWidth, tileHeight, 1,
				this->Image_Format, GL_UNSIGNED_BYTE, tile.data());
		}
	}



	// Set textre wrap and filter modes
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, this->Wrap_S);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, this->Wrap_T);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_R, this->Wrap_T); // If I need to modify this, make sure I create a Wrap_R

	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, this->Filter_Min);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, this->Filter_Max);
	// Unbind texture
	glBindTexture(GL_TEXTURE_2D_ARRAY, 0);
}

void Texture2D::Bind() const
{
	if (!this->isArray)
	{
		glBindTexture(GL_TEXTURE_2D, this->ID);
	}
	else
	{
		glBindTexture(GL_TEXTURE_2D_ARRAY, this->ID);
	}
}
