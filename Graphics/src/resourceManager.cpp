#include "resourceManager.hpp"


#include <iostream>
#include <sstream>
#include <fstream>
#include <asset_load.hpp>
#include <texture_asset.hpp>
#include <alext.h>

namespace Ebony
{
	std::unordered_map<std::string, Texture2D> ResourceManager::Textures;
	std::unordered_map<std::string, Shader> ResourceManager::Shaders;
	std::unordered_map<std::string, ALuint> ResourceManager::SoundEffectBuffers;
	std::unordered_map<std::string, EbonyAudio::MusicSource> ResourceManager::Music;

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

	ALuint ResourceManager::LoadSoundEffect(const std::string& file, const char* name)
	{
		ALenum err = 0, format = 0;
		ALuint buffer = 0;
		SNDFILE* sndfile = nullptr;
		SF_INFO sfinfo{};
		short* membuf = nullptr;
		sf_count_t num_frames = 0;
		ALsizei num_bytes = 0;

		/* Open the audio file and check that it's usable. */
		sndfile = sf_open(("../Audio/" + file).c_str(), SFM_READ, &sfinfo);
		if (!sndfile)
		{
			std::cerr << "Could not open audio in " << file << sf_strerror(sndfile) << std::endl;
			return 0;
		}
		if (sfinfo.frames < 1)
		{
			std::cerr << "Bad sample count in " << file << " " << sfinfo.frames << std::endl;
			//fprintf(stderr, "Bad sample count in %s (%" PRId64 ")\n", filename, sfinfo.frames);
			sf_close(sndfile);
			return 0;
		}

		/* Figure out the OpenAL format from the file and desired sample type. */
		format = AL_NONE;
		if (sfinfo.channels == 1)
		{
			format = AL_FORMAT_MONO16;
		}
		else if (sfinfo.channels == 2)
		{
			format = AL_FORMAT_STEREO16;
		}
		else if (sfinfo.channels == 3)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			{
				format = AL_FORMAT_BFORMAT2D_16;
			}
		}
		else if (sfinfo.channels == 4)
		{
			if (sf_command(sndfile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			{
				format = AL_FORMAT_BFORMAT3D_16;
			}
		}
		if (!format)
		{
			fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
			sf_close(sndfile);
			return 0;
		}
		/* Decode the whole audio file to a buffer. */
		membuf = static_cast<short*>(malloc((size_t)(sfinfo.frames * sfinfo.channels) * sizeof(short)));
		num_frames = sf_readf_short(sndfile, membuf, sfinfo.frames);

		if (num_frames < 1)
		{
			free(membuf);
			sf_close(sndfile);
			std::cerr << "Failed to read samples in " << file << " " << num_frames << std::endl;
			//fprintf(stderr, "Failed to read samples in %s (%" PRId64 ")\n", filename, num_frames);
			return 0;
		}
		num_bytes = (ALsizei)(num_frames * sfinfo.channels) * (ALsizei)sizeof(short);


		/* Buffer the audio data into a new buffer object, then free the data and
		 * close the file.
		 */
		buffer = 0;
		alGenBuffers(1, &buffer);
		alBufferData(buffer, format, membuf, num_bytes, sfinfo.samplerate);

		free(membuf);
		sf_close(sndfile);

		/* Check if an error occurred, and clean up if so. */
		err = alGetError();
		if (err != AL_NO_ERROR)
		{
			fprintf(stderr, "OpenAL Error: %s\n", alGetString(err));
			if (buffer && alIsBuffer(buffer))
				alDeleteBuffers(1, &buffer);
			return 0;
		}

		SoundEffectBuffers[name] = buffer;

		return buffer;
	}

	void  ResourceManager::UnloadSoundEffect(const char* name)
	{
		SoundEffectBuffers.erase(name);
	}

	ALuint ResourceManager::GetSoundEffect(const char* name)
	{
		return SoundEffectBuffers[name];
	}

	EbonyAudio::MusicSource& ResourceManager::LoadMusic(const std::string& file, const char* name)
	{
		Music[name] = EbonyAudio::MusicSource::LoadFromFile("../Audio/" + file);
		return Music[name];
	}

	void  ResourceManager::UnloadMusic(const char* name)
	{
		Music.erase(name);
	}

	EbonyAudio::MusicSource& ResourceManager::GetMusic(const char* name)
	{
		return Music[name];
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