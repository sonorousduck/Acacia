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

		static Shader LoadShader(const char* vShaderFile, const char* fShaderFile, std::string& name);
		static Shader GetShader(std::string& name);
		static void UnloadShader(std::string& name);


		static Texture2D LoadTexture(const char* file, std::string& name);
		static Texture2D GetTexture(std::string& name);
		static void UnloadTexture(std::string& name);


		// Unallocates all resources
		static void Clear();

	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		ResourceManager() {}

		static Shader loadShaderFromFile();

		static Texture2D loadTextureFromFile(char const* path);

	};
}