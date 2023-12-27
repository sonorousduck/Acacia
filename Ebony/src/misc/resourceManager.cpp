#include "resourceManager.hpp"


#include <iostream>
#include <sstream>
#include <fstream>
#include <asset_load.hpp>
#include <texture_asset.hpp>
#include "ThreadPool.hpp"


namespace Ebony
{
	std::unordered_map<std::string, std::shared_ptr<Texture2D>> ResourceManager::Textures;
	std::unordered_map<std::string, Shader> ResourceManager::Shaders;
	std::unordered_map<std::string, Mix_Chunk*> ResourceManager::SoundEffectBuffers;
	std::unordered_map<std::string, Mix_Music*> ResourceManager::MusicSources;
	std::unordered_map<std::string, std::shared_ptr<SpriteFont>> ResourceManager::Fonts;

	std::atomic_uint16_t ResourceManager::tasksRemaining{ 0 };

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


	std::shared_ptr<Texture2D> ResourceManager::LoadTexture(const std::string& file, const char* name, const std::string& nameOfGame, bool currentFolder, const std::string& otherFolder)
	{
		if (currentFolder)
		{
			Textures[name] = loadTextureFromFile("../" + nameOfGame + "/textures/" + file);
			return Textures[name];
		}
		Textures[name] = loadTextureFromFile("../Graphics/" + file);
		return Textures[name];
	}

	std::shared_ptr<Texture2D> ResourceManager::LoadAtlas(const std::string& file, const char* name, const std::string& nameOfGame, std::uint16_t tilesX, std::uint16_t tilesY)
	{
		Textures[name] = loadAtlasFromFileAs3D("../" + nameOfGame + "/textures/" + file, tilesX, tilesY);
		return Textures[name];
	}

	std::shared_ptr<Texture2D> ResourceManager::GetTexture(const char* name)
	{
		return Textures[name];
	}

	void ResourceManager::UnloadTexture(const char* name)
	{
		Textures.erase(name);
	}

	std::shared_ptr<Texture2D> ResourceManager::loadTextureFromFile(const std::string& path)
	{
		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(textureID);

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
			texture->Internal_Format = format;
			texture->Image_Format = format;
			texture->Generate(width, height, data.data());

			return texture;
		}

		return texture;
	}

	std::shared_ptr<Texture2D> ResourceManager::loadAtlasFromFile(char const* path, std::uint16_t tilesX, std::uint16_t tilesY)
	{
		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		std::shared_ptr<Texture2D> texture = std::make_shared<Texture2D>(textureID);

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
			texture->Internal_Format = format;
			texture->Image_Format = format;

			texture->Generate(width, height, data.data());

			texture->isAtlas = true;
			texture->imageCountX = tilesX;
			texture->imageCountY = tilesY;
			texture->Offset = { 1.0f / tilesX, 1.0f / tilesY };
			

			return texture;
		}

		return texture;
	}

	std::shared_ptr<Texture2D> ResourceManager::loadAtlasFromFileAs3D(const std::string& path, std::uint16_t tilesX, std::uint16_t tilesY)
	{
		unsigned int textureID = 0;
		glGenTextures(1, &textureID);

		std::shared_ptr texture = std::make_shared<Texture2D>(textureID);

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
			texture->Internal_Format = format;
			texture->Image_Format = format;
			
			
			
			texture->Generate3D(width, height, data.data(), tilesX, tilesY);

			return texture;
		}

		return texture;
	}

	Mix_Chunk* ResourceManager::LoadSoundEffect(const std::string& name, const std::string& filename, bool currentFolder, const std::string& otherFolder)
	{
		if (currentFolder)
		{
			SoundEffectBuffers[name] = Mix_LoadWAV(("../BrickBreaker/soundeffects/" + filename).c_str());
		}
		else
		{
			SoundEffectBuffers[name] = Mix_LoadWAV((otherFolder + filename).c_str());
		}

		return SoundEffectBuffers[name];
	}

	void  ResourceManager::UnloadSoundEffect(const std::string& name)
	{
		Mix_FreeChunk(SoundEffectBuffers[name]);
		SoundEffectBuffers[name] = NULL;
		SoundEffectBuffers.erase(name);
	}

	Mix_Chunk* ResourceManager::GetSoundEffect(const std::string& name)
	{
		return SoundEffectBuffers[name];
	}

	void ResourceManager::LoadMusic(const std::string& name, const std::string& filename, bool currentFolder, const std::string& otherFolder)
	{
		if (currentFolder)
		{
			MusicSources[name] = Mix_LoadMUS(("../BrickBreaker/music/" + filename).c_str());
		}
		else
		{
			MusicSources[name] = Mix_LoadMUS((otherFolder + filename).c_str());
		}
	}

	void ResourceManager::UnloadMusic(const char* name)
	{
		Mix_FreeMusic(MusicSources[name]);
		MusicSources[name] = NULL;
		MusicSources.erase(name);
	}

	Mix_Music* ResourceManager::GetMusic(const char* name)
	{
		return MusicSources[name];
	}

	void ResourceManager::LoadFont(const std::string& file, const std::string& name, const std::string& nameOfGame, bool currentFolder, const std::string& otherFolder)
	{
		std::shared_ptr<Ebony::SpriteFont> spriteFont = std::make_shared<Ebony::SpriteFont>();
		if (currentFolder)
		{
			spriteFont->LoadFont("../" + nameOfGame + "/fonts/" + file);

			Fonts[name] = spriteFont;
			return;
		}
		spriteFont->LoadFont("../Graphics/" + file);
		Fonts[name] = spriteFont;
		return;

	}

	void ResourceManager::UnloadFont(const char* name)
	{
		Fonts.erase(name);
	}

	std::shared_ptr<Ebony::SpriteFont> ResourceManager::GetFont(const char* name)
	{
		return Fonts[name];
	}

	void ResourceManager::LoadTextureAsync(const std::string& file, const char* name, std::function<void(std::string)> onComplete)
	{
		auto task = [=]()
		{
			std::cout << "Starting to load texture" << std::endl;

			Textures[name] = loadTextureFromFile("../Graphics/" + file);
			ResourceManager::loadComplete(file, onComplete);
		};

		ResourceManager::tasksRemaining++;
		auto threadTask = ThreadPool::instance().createTask(task);
		ThreadPool::instance().enqueueTask(threadTask);
	}

	void ResourceManager::LoadAtlasAsync(const std::string& file, const char* name, std::uint16_t tilesX, std::uint16_t tilesY, std::function<void(std::string)> onComplete)
	{
		auto task = [=]()
		{
			std::cout << "Starting to load texture atlas" << std::endl;
			Textures[name] = loadAtlasFromFileAs3D("../Graphics/" + file, tilesX, tilesY);
			ResourceManager::loadComplete(file, onComplete);
		};

		ResourceManager::tasksRemaining++;
		auto threadTask = ThreadPool::instance().createTask(task);
		ThreadPool::instance().enqueueTask(threadTask);
	}

	void ResourceManager::LoadShaderAsync(const std::string& vShaderFile, const std::string& fShaderFile, const char* name, std::function<void(std::string)> onComplete)
	{

	}

	/*void ResourceManager::LoadSoundEffectAsync(const std::string& file, const char* name, std::function<void(std::string)> onComplete)
	{
		auto task = [=]()
		{
			std::cout << "Starting to load sound effect" << std::endl;

			SoundEffectBuffers[name] = SoundBuffer::get()->addSoundEffect(("../Audio/" + file).c_str());;
			ResourceManager::loadComplete(file, onComplete);
		};

		ResourceManager::tasksRemaining++;
		auto threadTask = ThreadPool::instance().createTask(task);
		ThreadPool::instance().enqueueTask(threadTask);
	}*/

	/*void ResourceManager::LoadMusicAsync(const std::string& file, const char* name, std::function<void(std::string)> onComplete)
	{
		auto task = [=]()
		{
			Music[name] = EbonyAudio::MusicSource::LoadFromFile("../Audio/" + file);
			ResourceManager::loadComplete(file, onComplete);
		};

		ResourceManager::tasksRemaining++;
		auto threadTask = ThreadPool::instance().createIOTask(task);
		ThreadPool::instance().enqueueTask(threadTask);
	}*/

	void ResourceManager::loadComplete(const std::string& file, std::function<void(std::string)> onComplete)
	{

		std::cout << "Finished loading: " << file << std::endl;
		tasksRemaining--;

		if (onComplete != nullptr)
		{
			std::cout << "Eventually, this load complete should contain more information and whether it failed" << std::endl;

			onComplete(file);
		}
		
	}

	void ResourceManager::Clear()
	{
		for (auto& iter : Shaders)
		{
			glDeleteProgram(iter.second.ID);
		}

		for (auto& iter : Textures)
		{
			glDeleteTextures(1, &iter.second->ID);
		}


		for (auto it = SoundEffectBuffers.begin(); it != SoundEffectBuffers.end(); it++)
		{
			//alDeleteBuffers(1, &it->second);
			Mix_FreeChunk(it->second);
			it->second = NULL;
		}

		for (auto it = MusicSources.begin(); it != MusicSources.end(); it++)
		{
			Mix_FreeMusic(it->second);
			it->second = NULL;
		}

		SoundEffectBuffers.clear();



	}





}