#include "resourceManager.hpp"


#include <iostream>
#include <sstream>
#include <fstream>
#include <asset_load.hpp>
#include <texture_asset.hpp>

namespace Ebony
{
	std::unordered_map<std::string, Texture2D> ResourceManager::Textures;
	std::unordered_map<std::string, Shader> ResourceManager::Shaders;

	Shader& ResourceManager::LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const char* name)
	{
		Shaders[name] = Shader("../Graphics/" + vShaderFile, "../Graphics/" + fShaderFile);
		return Shaders[name];
	}

	Shader& ResourceManager::GetShader(const char* name)
	{
		return Shaders[name];
	}

	void ResourceManager::UnloadShader(const char* name)
	{
		Shaders.erase(name);
	}


	Texture2D& ResourceManager::LoadTexture(const std::string& file, const char* name)
	{
		Textures[name] = loadTextureFromFile("../Graphics/" + file);
		return Textures[name];
	}

	Texture2D& ResourceManager::LoadAtlas(const std::string& file, const char* name, std::uint16_t tilesX, std::uint16_t tilesY)
	{
		Textures[name] = loadAtlasFromFileAs3D("../Graphics/" + file, tilesX, tilesY);
		return Textures[name];
	}

	Texture2D& ResourceManager::GetTexture(const char* name)
	{
		return Textures[name];
	}

	void ResourceManager::UnloadTexture(const char* name)
	{
		Textures.erase(name);
	}

	Texture2D ResourceManager::loadTextureFromFile(const std::string& path)
	{
		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		Texture2D texture = Texture2D(textureID);

		assets::AssetFile file{};
		bool loaded = assets::load_binaryfile(path.c_str(), file);

		if (!loaded)
		{
			std::cout << "Error when loading image: " << path << std::endl;
			exit(0);
		}

		assets::TextureInfo textureInfo = assets::read_texture_info(&file);

		int nrComponents = textureInfo.textureFormat;
		int width = textureInfo.pixelSize[0];
		int height = textureInfo.pixelSize[1];

		if (file.binaryBlob.data())
		{
			GLenum format{};

			switch (nrComponents)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				break;
			}

			std::vector<char> data(textureInfo.textureSize);

			assets::unpack_texture(&textureInfo, file.binaryBlob.data(), file.binaryBlob.size(), data.data());
			texture.Internal_Format = format;
			texture.Image_Format = format;
			texture.Generate(width, height, data.data());

			return texture;
		}

		return texture;
	}

	Texture2D ResourceManager::loadAtlasFromFile(char const* path, std::uint16_t tilesX, std::uint16_t tilesY)
	{
		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		Texture2D texture = Texture2D(textureID);

		assets::AssetFile file{};
		bool loaded = assets::load_binaryfile(path, file);

		if (!loaded)
		{
			std::cout << "Error when loading image: " << path << std::endl;
			exit(0);
		}

		assets::TextureInfo textureInfo = assets::read_texture_info(&file);

		int nrComponents = textureInfo.textureFormat;
		int width = textureInfo.pixelSize[0];
		int height = textureInfo.pixelSize[1];

		if (file.binaryBlob.data())
		{
			GLenum format{};

			switch (nrComponents)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				break;
			}

			std::vector<char> data(textureInfo.textureSize);

			assets::unpack_texture(&textureInfo, file.binaryBlob.data(), file.binaryBlob.size(), data.data());
			texture.Internal_Format = format;
			texture.Image_Format = format;

			texture.Generate(width, height, data.data());

			texture.isAtlas = true;
			texture.imageCountX = tilesX;
			texture.imageCountY = tilesY;
			texture.Offset = { 1.0f / tilesX, 1.0f / tilesY };
			

			return texture;
		}

		return texture;
	}

	Texture2D ResourceManager::loadAtlasFromFileAs3D(const std::string& path, std::uint16_t tilesX, std::uint16_t tilesY)
	{
		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		Texture2D texture = Texture2D(textureID);

		assets::AssetFile file{};
		bool loaded = assets::load_binaryfile(path.c_str(), file);

		if (!loaded)
		{
			std::cout << "Error when loading image: " << path << std::endl;
			exit(0);
		}

		assets::TextureInfo textureInfo = assets::read_texture_info(&file);

		int nrComponents = textureInfo.textureFormat;
		int width = textureInfo.pixelSize[0];
		int height = textureInfo.pixelSize[1];

		if (file.binaryBlob.data())
		{
			GLenum format{};

			switch (nrComponents)
			{
			case 1:
				format = GL_RED;
				break;
			case 3:
				format = GL_RGB;
				break;
			case 4:
				format = GL_RGBA;
				break;
			default:
				break;
			}

			std::vector<char> data(textureInfo.textureSize);

			assets::unpack_texture(&textureInfo, file.binaryBlob.data(), file.binaryBlob.size(), data.data());
			texture.Internal_Format = format;
			texture.Image_Format = format;
			
			
			
			texture.Generate3D(width, height, data.data(), tilesX, tilesY);

			return texture;
		}

		return texture;
	}


	void ResourceManager::Clear()
	{
		for (auto& iter : Shaders)
		{
			glDeleteProgram(iter.second.ID);
		}

		for (auto& iter : Textures)
		{
			glDeleteTextures(1, &iter.second.ID);
		}
	}



}