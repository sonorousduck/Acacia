#pragma once


#include <unordered_map>
#include <string>

#include <glad/glad.h>

#include "texture.hpp"
#include "shader.hpp"


namespace Ebony
{


	class ResourceManager
	{
	public:
		static std::unordered_map<std::string, Shader> Shaders;
		static std::unordered_map<std::string, Texture2D> Textures;

		static Shader& LoadShader(const char* vShaderFile, const char* fShaderFile, const char* name);
		static Shader& GetShader(const char* name);
		static void UnloadShader(const char* name);


		static Texture2D& LoadTexture(const char* file, const char* name);
		static Texture2D& LoadAtlas(const char* file, const char* name, std::uint16_t tilesX, std::uint16_t tilesY);
		static Texture2D& GetTexture(const char* name);
		static void UnloadTexture(const char* name);


		// Unallocates all resources
		static void Clear();

	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		ResourceManager() {}

		//static Shader& loadShaderFromFile();

		static Texture2D loadTextureFromFile(char const* path);
		static Texture2D loadAtlasFromFile(char const* path, std::uint16_t tilesX, std::uint16_t tilesY);
		static Texture2D loadAtlasFromFileAs3D(char const* path, std::uint16_t tilesX, std::uint16_t tilesY);

	};
}