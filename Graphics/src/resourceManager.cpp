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

	Shader ResourceManager::LoadShader(const char* vShaderFile, const char* fShaderFile, std::string& name)
	{
		Shaders[name] = Shader(vShaderFile, fShaderFile);
		return Shaders[name];
	}

	Shader ResourceManager::GetShader(std::string& name)
	{
		return Shaders[name];
	}

	void ResourceManager::UnloadShader(std::string& name)
	{
		Shaders.erase(name);
	}


	Texture2D ResourceManager::LoadTexture(const char* file, std::string& name)
	{
		Textures[name] = loadTextureFromFile(file);
		return Textures[name];
	}

	Texture2D ResourceManager::GetTexture(std::string& name)
	{
		return Textures[name];
	}

	void ResourceManager::UnloadTexture(std::string& name)
	{
		Textures.erase(name);
	}

	Texture2D ResourceManager::loadTextureFromFile(char const* path)
	{

		Texture2D texture = Texture2D();


		assets::AssetFile file{};
		bool loaded = assets::load_binaryfile(path, file);

		if (!loaded)
		{
			std::cout << "Error when loading image: " << path << std::endl;
			return -1;
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

			texture.Generate(width, height, data.data());

			return texture;
		}

		return texture;
	}

	void ResourceManager::Clear()
	{
		for (auto iter : Shaders)
		{
			glDeleteProgram(iter.second.ID);
		}

		for (auto iter : Textures)
		{
			glDeleteTextures(1, &iter.second.ID);
		}
	}



}