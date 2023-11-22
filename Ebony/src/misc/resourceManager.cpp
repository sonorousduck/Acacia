#include "resourceManager.hpp"


#include <iostream>
#include <sstream>
#include <fstream>
#include <asset_load.hpp>
#include <texture_asset.hpp>
#include <alext.h>
#include "ThreadPool.hpp"
#include "../../Audio/src/SoundBuffer.hpp"
#include "../../../Audio/src/audioManager.hpp"

namespace Ebony
{
	std::unordered_map<std::string, Texture2D> ResourceManager::Textures;
	std::unordered_map<std::string, Shader> ResourceManager::Shaders;
	std::unordered_map<std::string, ALuint> ResourceManager::SoundEffectBuffers;
	std::unordered_map<std::string, std::shared_ptr<EbonyAudio::MusicSource>> ResourceManager::MusicSources;
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


	Texture2D& ResourceManager::LoadTexture(const std::string& file, const char* name, bool currentFolder, const std::string& otherFolder)
	{
		if (currentFolder)
		{
			Textures[name] = loadTextureFromFile("../BrickBreaker/textures/" + file);
			return Textures[name];
		}
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

	ALuint ResourceManager::LoadSoundEffect(const std::string& name, const std::string& filename, bool currentFolder, const std::string& otherFolder)
	{
		ALenum err = 0, format = 0;
		ALuint buffer = 0;
		SNDFILE* sndfile = nullptr;
		SF_INFO sfinfo{};
		short* membuf = nullptr;
		sf_count_t num_frames = 0;
		ALsizei num_bytes = 0;

		/* Open the audio file and check that it's usable. */
		if (currentFolder)
		{
			sndfile = sf_open(("../BrickBreaker/soundeffects/" + filename).c_str(), SFM_READ, &sfinfo);
		}
		else
		{
			sndfile = sf_open((otherFolder + filename).c_str(), SFM_READ, &sfinfo);
		}
		if (!sndfile)
		{
			std::cerr << "Could not open audio in " << filename << ":\n" << sf_strerror(sndfile) << "\n";
			//fprintf(stderr, "Could not open audio in %s: %s\n", filename, sf_strerror(sndfile));
			return 0;
		}
		if (sfinfo.frames < 1)
		{
			std::cerr << "Bad sample count in " << filename << "(" << sfinfo.frames << ")\n";
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
			std::cerr << "Unsupported channel count: " << sfinfo.channels << "\n";
			//fprintf(stderr, "Unsupported channel count: %d\n", sfinfo.channels);
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
			std::cerr << "Failed to read samples in " << filename << "(" << num_frames << ")\n";
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

	void  ResourceManager::UnloadSoundEffect(const std::string& name)
	{
		SoundEffectBuffers.erase(name);
	}

	ALuint ResourceManager::GetSoundEffect(const std::string& name)
	{
		return SoundEffectBuffers[name];
	}

	std::shared_ptr<EbonyAudio::MusicSource> ResourceManager::LoadMusic(const std::string& name, const std::string& filename, bool currentFolder, const std::string& otherFolder)
	{
		std::shared_ptr<EbonyAudio::MusicSource> musicSource = std::make_shared<EbonyAudio::MusicSource>();
		musicSource->fileFormat = EbonyAudio::AudioFileFormat::OTHER;

		//alGenSources(1, &musicSource->source);
		alGenBuffers(musicSource->NUM_BUFFERS, musicSource->buffers);

		std::size_t frame_size{};

		if (currentFolder)
		{
			musicSource->sndFile = sf_open(("../BrickBreaker/music/" + filename).c_str(), SFM_READ, &musicSource->sfInfo);
		}
		else
		{
			musicSource->sndFile = sf_open((otherFolder + filename).c_str(), SFM_READ, &musicSource->sfInfo);

		}

		if (!musicSource->sndFile)
		{
			throw("Could not open provided music file. Check path");
		}

		// Get the sound format and figure out the OpenAL format

		switch (musicSource->sfInfo.channels)
		{
		case 1:
			musicSource->format = AL_FORMAT_MONO16;
			break;
		case 2:
			musicSource->format = AL_FORMAT_STEREO16;
			break;
		case 3:
			if (sf_command(musicSource->sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			{
				musicSource->format = AL_FORMAT_BFORMAT3D_16;
			}
			break;
		case 4:
			if (sf_command(musicSource->sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
			{
				musicSource->format = AL_FORMAT_BFORMAT3D_16;
			}
			break;
		default:
			throw("Unsupported number of channels");
			break;
		}


		if (!musicSource->format)
		{
			sf_close(musicSource->sndFile);
			musicSource->sndFile = NULL;
			throw("Unsupported channel count from file");
		}

		frame_size = ((static_cast<size_t>(musicSource->BUFFER_SAMPLES) * static_cast<size_t>(musicSource->sfInfo.channels))) * sizeof(short);
		musicSource->memBuf = static_cast<short*>(malloc(frame_size));


		MusicSources[name] = musicSource;

		return musicSource;
	}

	void  ResourceManager::UnloadMusic(const char* name)
	{
		MusicSources.erase(name);
	}

	std::shared_ptr<EbonyAudio::MusicSource> ResourceManager::GetMusic(const char* name)
	{
		return MusicSources[name];
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
			glDeleteTextures(1, &iter.second.ID);
		}


		for (auto it = SoundEffectBuffers.begin(); it != SoundEffectBuffers.end(); it++)
		{
			alDeleteBuffers(1, &it->second);
		}

		SoundEffectBuffers.clear();



	}





}