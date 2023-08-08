#pragma once


#include <unordered_map>
#include <string>

#include <glad/glad.h>

#include "texture.hpp"
#include "shader.hpp"
#include "../Audio/src/MusicSource.hpp"
#include "../Audio/src/SoundSource.hpp"

namespace Ebony
{


	class ResourceManager
	{
	public:
		static std::unordered_map<std::string, Shader> Shaders;
		static std::unordered_map<std::string, Texture2D> Textures;
		static std::unordered_map<std::string, ALuint> SoundEffectBuffers;
		static std::unordered_map<std::string, EbonyAudio::MusicSource> Music;

		static Shader& LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const char* name);
		static Shader& GetShader(const char* name);
		static void UnloadShader(const char* name);


		static Texture2D& LoadTexture(const std::string& file, const char* name);
		static Texture2D& LoadAtlas(const std::string& file, const char* name, std::uint16_t tilesX, std::uint16_t tilesY);
		static Texture2D& GetTexture(const char* name);
		static void UnloadTexture(const char* name);

		static ALuint LoadSoundEffect(const std::string& file, const char* name);
		static void UnloadSoundEffect(const char* name);
		static ALuint GetSoundEffect(const char* name);

		static EbonyAudio::MusicSource& LoadMusic(const std::string& file, const char* name);
		static void UnloadMusic(const char* name);
		static EbonyAudio::MusicSource& GetMusic(const char* name);
		
		


		// Unallocates all resources
		static void Clear();

	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		ResourceManager() {}

		//static Shader& loadShaderFromFile();

		static Texture2D loadTextureFromFile(const std::string& path);
		static Texture2D loadAtlasFromFile(char const* path, std::uint16_t tilesX, std::uint16_t tilesY);
		static Texture2D loadAtlasFromFileAs3D(const std::string& path, std::uint16_t tilesX, std::uint16_t tilesY);

	};
}