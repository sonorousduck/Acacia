#pragma once
#include "AL/al.h"
#include <sndfile.h>
#include "alhelpers.hpp"
#include <iostream>
#include <utility>
//#include "../ThirdParty/minimp3/minimp3_ex.h"


namespace EbonyAudio
{
	enum class AudioFileFormat
	{
		NONE = 0,
		MP3,
		OTHER
	};


	class MusicSource
	{
	public:
		~MusicSource();

		MusicSource() = default;
		MusicSource(uint32_t handle, bool loaded, float length);
		//void Pause();
		//void Stop();

		bool IsLoaded() const { return m_Loaded; }

		void SetPosition(float x, float y, float z);
		void SetGain(float gain);
		void SetPitch(float pitch);
		void SetSpatial(bool spatial);
		void SetLoop(bool loop);
		void UpdateBufferStream();

		static ALenum GetOpenALFormat(uint32_t channels);

		std::pair<uint32_t, uint32_t> GetLengthMinutesAndSeconds() const;

		static MusicSource LoadFromFile(const std::string& file, bool spatial = false);

		AudioFileFormat fileFormat;

	private:


		uint32_t m_BufferHandle = 0;
		bool m_Loaded = false;
		bool m_Spatial = false;

		float m_TotalDuration = 0;

		// Attributes
		float m_Position[3] = { 0.0f, 0.0f, 0.0f };
		float m_Gain = 1.0f;
		float m_Pitch = 1.0f;
		bool m_Loop = false;

		friend class Music;

		static const int NUM_BUFFERS = 4;
		static const int BUFFER_SAMPLES = 8192;
		ALuint source{};
		ALuint buffers[NUM_BUFFERS]{};
		
		SNDFILE* sndFile{};
		SF_INFO sfInfo{};

		//FILE* mp3File{};
		//mp3dec_file_info_t mp3Info{};

		short* memBuf{};
		//short pcm[MINIMP3_MAX_SAMPLES_PER_FRAME];
		ALenum format{};


		//MusicSource() = delete;
	};


	class Music
	{
	public:
		static void Init();

		static MusicSource LoadMusicSource(const std::string& filename);
		static void Play(MusicSource& source);

	private:
		static MusicSource LoadMusicSourceOther(const std::string& filename);
		//static MusicSource LoadMusicSourceMP3(const std::string& filename);
	};
}