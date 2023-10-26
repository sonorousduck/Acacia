#include "MusicSource.hpp"
#include <cstddef>
#include <alext.h>
#include <stdio.h>
#include <string>
#include <iostream>

//#define MINIMP3_IMPLEMENTATION
//#include "../ThirdParty/minimp3/minimp3.h"
//#include "../ThirdParty/minimp3/minimp3_ex.h"

#include <filesystem>


//static mp3dec_t s_Mp3d;
static ALCdevice* s_AudioDevice = nullptr;

static uint8_t* s_AudioScratchBuffer;
static uint32_t s_AudioScratchBufferSize = 10 * 1024 * 1024; // 10mb initially



static EbonyAudio::AudioFileFormat GetFileFormat(const std::string& filename)
{
	std::filesystem::path path = filename;
	std::string extension = path.extension().string();

	if (extension == ".ogg" || extension == ".wav" || extension == ".mp3")  return EbonyAudio::AudioFileFormat::OTHER;
	//if (extension == ".mp3")  return EbonyAudio::AudioFileFormat::MP3;

	return EbonyAudio::AudioFileFormat::NONE;
}

ALenum EbonyAudio::MusicSource::GetOpenALFormat(uint32_t channels)
{
	// Note: Mp3 will always be 1 or 2 bits, but sndFile may be 3 or 4.
	switch (channels)
	{
	case 1:
		return AL_FORMAT_MONO16;
	case 2:
		return AL_FORMAT_STEREO16;
	}
	return 0;
}


EbonyAudio::MusicSource EbonyAudio::Music::LoadMusicSourceOther(const std::string& filename)
{
	EbonyAudio::MusicSource musicSource{};
	musicSource.fileFormat = EbonyAudio::AudioFileFormat::OTHER;

	alGenSources(1, &musicSource.source);
	alGenBuffers(musicSource.NUM_BUFFERS, musicSource.buffers);

	std::size_t frame_size{};

	musicSource.sndFile = sf_open(filename.c_str(), SFM_READ, &musicSource.sfInfo);

	if (!musicSource.sndFile)
	{
		throw("Could not open provided music file. Check path");
	}

	// Get the sound format and figure out the OpenAL format

	switch (musicSource.sfInfo.channels)
	{
	case 1:
		musicSource.format = AL_FORMAT_MONO16;
	case 2:
		musicSource.format = AL_FORMAT_STEREO16;
	case 3:
		if (sf_command(musicSource.sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			musicSource.format = AL_FORMAT_BFORMAT3D_16;
		}
	case 4:
		if (sf_command(musicSource.sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			musicSource.format = AL_FORMAT_BFORMAT3D_16;
		}
	}


	if (!musicSource.format)
	{
		sf_close(musicSource.sndFile);
		musicSource.sndFile = NULL;
		throw("Unsupported channel count from file");
	}

	frame_size = ((static_cast<size_t>(musicSource.BUFFER_SAMPLES) * static_cast<size_t>(musicSource.sfInfo.channels))) * sizeof(short);
	musicSource.memBuf = static_cast<short*>(malloc(frame_size));

	return musicSource;
}

//EbonyAudio::MusicSource EbonyAudio::Music::LoadMusicSourceMP3(const std::string& filename)
//{
	//EbonyAudio::MusicSource musicSource{};
	/*musicSource.fileFormat = EbonyAudio::AudioFileFormat::MP3;
	mp3dec_file_info_t info{};

	alGenSources(1, &musicSource.source);
	alGenBuffers(musicSource.NUM_BUFFERS, musicSource.buffers);

	musicSource.mp3File = fopen(filename.c_str(), "r");

	int loadResult = mp3dec_load(&s_Mp3d, filename.c_str(), &info, NULL, NULL);
	musicSource.mp3Info = info;

	switch (info.channels)
	{
	case 1:
		musicSource.format = AL_FORMAT_MONO16;
		break;
	case 2:
		musicSource.format = AL_FORMAT_STEREO16;
		break;
	case 3:
		if (sf_command(musicSource.sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			musicSource.format = AL_FORMAT_BFORMAT3D_16;
		}
		break;
	case 4:
		if (sf_command(musicSource.sndFile, SFC_WAVEX_GET_AMBISONIC, NULL, 0) == SF_AMBISONIC_B_FORMAT)
		{
			musicSource.format = AL_FORMAT_BFORMAT3D_16;
		}
		break;
	default:
		break;
	}
	
	if (!musicSource.format)
	{
		throw("Unsupported channel count from file");
	}

	std::size_t frame_size = ((static_cast<size_t>(musicSource.BUFFER_SAMPLES) * static_cast<size_t>(musicSource.mp3Info.channels))) * sizeof(short);
	musicSource.memBuf = static_cast<short*>(malloc(frame_size));*/

	//return musicSource;


	/*mp3dec_file_info_t info{};

	int loadResult = mp3dec_load(&s_Mp3d, filename.c_str(), &info, NULL, NULL);
	uint32_t size = info.samples * sizeof(mp3d_sample_t);

	auto sampleRate = info.hz;
	auto channels = info.channels;
	auto alFormat = MusicSource::GetOpenALFormat(channels);
	float lengthSeconds = size / (info.avg_bitrate_kbps * 1024.0f);

	ALuint buffer{};
	alGenBuffers(1, &buffer);
	alBufferData(buffer, alFormat, info.buffer, size, sampleRate);

	MusicSource result = { buffer, true, lengthSeconds };
	result.fileFormat = AudioFileFormat::MP3;
	alGenSources(1, &result.source);
	alSourcei(result.source, AL_BUFFER, buffer);

	if (alGetError() != AL_NO_ERROR)
		std::cout << "Failed to setup sound source" << std::endl;

	return result;*/
//}

void EbonyAudio::Music::Init()
{
	if (InitAL(s_AudioDevice, nullptr, 0) != 0)
	{
		std::cout << "Audio Device error!" << std::endl;
	}
	//mp3dec_init(&s_Mp3d);
	
	s_AudioScratchBuffer = new uint8_t[s_AudioScratchBufferSize];

	// Init listener
	ALfloat listenerPos[] = { 0.0,0.0,0.0 };
	ALfloat listenerVel[] = { 0.0,0.0,0.0 };
	ALfloat listenerOri[] = { 0.0,0.0,-1.0, 0.0,1.0,0.0 };
	alListenerfv(AL_POSITION, listenerPos);
	alListenerfv(AL_VELOCITY, listenerVel);
	alListenerfv(AL_ORIENTATION, listenerOri);
}

EbonyAudio::MusicSource EbonyAudio::Music::LoadMusicSource(const std::string& filename)
{
	auto format = GetFileFormat(filename);

	switch (format)
	{
	case AudioFileFormat::OTHER: return LoadMusicSourceOther(filename);
	//case AudioFileFormat::MP3: return LoadMusicSourceMP3(filename);
	}
}


void EbonyAudio::Music::Play(EbonyAudio::MusicSource& musicSource)
{
	if (musicSource.fileFormat == AudioFileFormat::OTHER)
	{
		 //Clear any AL errors
		alGetError();

		// Rewind the source position and clear the buffer queue
		alSourceRewind(musicSource.source);
		alSourcei(musicSource.source, AL_BUFFER, 0);


		for (ALsizei i = 0; i < musicSource.NUM_BUFFERS; i++)
		{
			sf_count_t slen = sf_readf_short(musicSource.sndFile, musicSource.memBuf, musicSource.BUFFER_SAMPLES);
			if (slen < 1) break;

			slen *= musicSource.sfInfo.channels * static_cast<sf_count_t>(sizeof(short));
			alBufferData(musicSource.buffers[i], musicSource.format, musicSource.memBuf, static_cast<ALsizei>(slen), musicSource.sfInfo.samplerate);
		}
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error Buffering for playback");
		}

		/* Now queue and start playback! */
		alSourceQueueBuffers(musicSource.source, musicSource.NUM_BUFFERS, musicSource.buffers);
		alSourcePlay(musicSource.source);
		if (alGetError() != AL_NO_ERROR)
		{
			throw("Error starting playback");
		}

	}
	else if (musicSource.fileFormat == AudioFileFormat::MP3)
	{
		////Clear any AL errors
		//alGetError();

		//// Rewind the source position and clear the buffer queue
		//alSourceRewind(musicSource.source);
		//alSourcei(musicSource.source, AL_BUFFER, 0);
		//for (ALsizei i = 0; i < musicSource.NUM_BUFFERS; i++)
		//{
		//	size_t n = fread(musicSource.inputBuf, 1, 16384, musicSource.mp3File);

		//	if (n == 0) break;

		//	

		//	//mp3dec_decode_frame(&s_Mp3d, musicSource.memBuf, musicSource.BUFFER_SAMPLES, NULL, NULL, NULL);

		//	//sf_count_t slen = sf_readf_short(musicSource.sndFile, musicSource.memBuf, musicSource.BUFFER_SAMPLES);
		//	//if (slen < 1) break;

		//	//slen *= musicSource.sfInfo.channels * static_cast<sf_count_t>(sizeof(short));
		//	//alBufferData(musicSource.buffers[i], musicSource.format, musicSource.memBuf, static_cast<ALsizei>(slen), musicSource.sfInfo.samplerate);
		//}
		//if (alGetError() != AL_NO_ERROR)
		//{
		//	throw("Error Buffering for playback");
		//}

		///* Now queue and start playback! */
		//alSourceQueueBuffers(musicSource.source, musicSource.NUM_BUFFERS, musicSource.buffers);
		//alSourcePlay(musicSource.source);
		//if (alGetError() != AL_NO_ERROR)
		//{
		//	throw("Error starting playback");
		//}




		//alSourcePlay(musicSource.source);
	}
}


EbonyAudio::MusicSource::MusicSource(uint32_t handle, bool loaded, float length)
	: m_BufferHandle(handle), m_Loaded(loaded), m_TotalDuration(length)
{
}

EbonyAudio::MusicSource::~MusicSource()
{
	if (source)
	{
		alDeleteSources(1, &source);
	}

	if (sndFile)
	{
		sf_close(sndFile);
	}

	sndFile = nullptr;
	free(memBuf);
	alDeleteBuffers(NUM_BUFFERS, buffers);
}


void EbonyAudio::MusicSource::SetPosition(float x, float y, float z)
{
	m_Position[0] = x;
	m_Position[1] = y;
	m_Position[2] = z;

	alSourcefv(source, AL_POSITION, m_Position);
}

void EbonyAudio::MusicSource::SetGain(float gain)
{
	m_Gain = gain;

	alSourcef(source, AL_GAIN, gain);
}

void EbonyAudio::MusicSource::SetPitch(float pitch)
{
	m_Pitch = pitch;

	alSourcef(source, AL_PITCH, pitch);
}

void EbonyAudio::MusicSource::SetSpatial(bool spatial)
{
	m_Spatial = spatial;

	alSourcei(source, AL_SOURCE_SPATIALIZE_SOFT, spatial ? AL_TRUE : AL_FALSE);
	alDistanceModel(AL_INVERSE_DISTANCE_CLAMPED);
}

void EbonyAudio::MusicSource::SetLoop(bool loop)
{
	m_Loop = loop;

	alSourcei(source, AL_LOOPING, loop ? AL_TRUE : AL_FALSE);
}

std::pair<uint32_t, uint32_t> EbonyAudio::MusicSource::GetLengthMinutesAndSeconds() const
{
	return { (uint32_t)(m_TotalDuration / 60.0f), (uint32_t)m_TotalDuration % 60 };
}

EbonyAudio::MusicSource EbonyAudio::MusicSource::LoadFromFile(const std::string& file, bool spatial)
{
	MusicSource result = Music::LoadMusicSource(file);
	result.SetSpatial(spatial);
	return result;
}

void EbonyAudio::MusicSource::UpdateBufferStream()
{
	if (fileFormat == AudioFileFormat::OTHER)
	{


		ALint processed = 0, state = 0;

		// Clear errors
		alGetError();

		// Get relevant source info
		alGetSourcei(source, AL_SOURCE_STATE, &state);
		alGetSourcei(source, AL_BUFFERS_PROCESSED, &processed);

		if (alGetError() != AL_NO_ERROR)
		{
			throw("error checking music source state");
		}

		// Unqueue and handle each processed buffer
		while (processed > 0)
		{
			ALuint bufid = 0;
			sf_count_t slen = 0;

			alSourceUnqueueBuffers(source, 1, &bufid);
			processed--;

			/* Read the next chunk of data, refill the buffer, and queue it
			 * back on the source */
			slen = sf_readf_short(sndFile, memBuf, BUFFER_SAMPLES);
			if (slen > 0)
			{
				slen *= sfInfo.channels * (sf_count_t)sizeof(short);
				alBufferData(bufid, format, memBuf, (ALsizei)slen,
					sfInfo.samplerate);
				alSourceQueueBuffers(source, 1, &bufid);
			}
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error buffering music data");
			}
		}

		/* Make sure the source hasn't underrun */
		if (state != AL_PLAYING && state != AL_PAUSED)
		{
			ALint queued;

			/* If no buffers are queued, playback is finished */
			alGetSourcei(source, AL_BUFFERS_QUEUED, &queued);
			if (queued == 0)
				return;

			alSourcePlay(source);
			if (alGetError() != AL_NO_ERROR)
			{
				throw("error restarting music playback");
			}
		}
	}
}