#pragma once


#include <unordered_map>
#include <string>
#include <functional>
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


		static Shader& LoadShader(const std::string& vShaderFile, const std::string& fShaderFile, const char* name);
		static Shader& GetShader(const char* name);
		static void UnloadShader(const char* name);


		static Texture2D& LoadTexture(const std::string& file, const char* name);
		static Texture2D& LoadAtlas(const std::string& file, const char* name, std::uint16_t tilesX, std::uint16_t tilesY);
		static Texture2D& GetTexture(const char* name);
		static void UnloadTexture(const char* name);


		static void LoadTextureAsync(const std::string& file, const char* name, std::function<void(std::string)> onComplete);
		static void LoadAtlasAsync(const std::string& file, const char* name, std::uint16_t tilesX, std::uint16_t tilesY, std::function<void(std::string)> onComplete);
		static void LoadShaderAsync(const std::string& vShaderFile, const std::string& fShaderFile, const char* name, std::function<void(std::string)> onComplete);
		static void LoadSoundEffectAsync(const std::string& file, const char* name, std::function<void(std::string)> onComplete);
		static void LoadMusicAsync(const std::string& file, const char* name, std::function<void(std::string)> onComplete);

		static std::uint32_t LoadSoundEffect(const std::string& file, const char* name);
		static void UnloadSoundEffect(const char* name);
		static std::uint32_t GetSoundEffect(const char* name);

		static EbonyAudio::MusicSource& LoadMusic(const std::string& file, const char* name);
		static void UnloadMusic(const char* name);
		static EbonyAudio::MusicSource& GetMusic(const char* name);
		
		static void loadComplete(const std::string& file, std::function<void(std::string)> onComplete);


		// Unallocates all resources
		static void Clear();

	private:
		// private constructor, that is we do not want any actual resource manager objects. Its members and functions should be publicly available (static).
		ResourceManager() {}

		//static Shader& loadShaderFromFile();

		static Texture2D loadTextureFromFile(const std::string& path);
		static Texture2D loadAtlasFromFile(char const* path, std::uint16_t tilesX, std::uint16_t tilesY);
		static Texture2D loadAtlasFromFileAs3D(const std::string& path, std::uint16_t tilesX, std::uint16_t tilesY);

		static std::unordered_map<std::string, Shader> Shaders;
		static std::unordered_map<std::string, Texture2D> Textures;
		static std::unordered_map<std::string, std::uint32_t> SoundEffectBuffers;
		static std::unordered_map<std::string, EbonyAudio::MusicSource> Music;
		
		static std::atomic_uint16_t tasksRemaining; // We need this so we are guarenteed not to start a level while content is loading

	};
}