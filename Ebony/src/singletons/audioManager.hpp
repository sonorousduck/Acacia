#pragma once

//#include "../Audio/src/SoundDevice.hpp"
//#include "../Audio/src/MusicSource.hpp"
//#include "../Audio/src/SoundSource.hpp"
//#include "../Audio/src/SourcePool.hpp"

#include "SDL_mixer.h"
#include <cassert>
#include <deque>
#include <cstdint>
#include "../Core.hpp"
#include <vector>


namespace Ebony
{
	enum AudioType : std::uint8_t
	{
		UI = BIT(0),
		ENTITY = BIT(1),
		MUSIC = BIT(2),
	};

	enum State : std::uint8_t
	{
		Stopped = BIT(0),
		Playing = BIT(1),
		Paused = BIT(2),
	};



	class AudioManager
	{
	public:

		static void Init(std::uint8_t channelCount = 32, std::uint8_t uiChannels = 6, std::uint8_t entityChannels = 26)
		{
			assert(uiChannels + entityChannels == channelCount);

			Mix_AllocateChannels(channelCount);

			uiChannelCount = uiChannels;
			entityChannelCount = entityChannels;
			sourceCount = channelCount;

			UIChannelPool = std::deque<int>();
			EntityChannelPool = std::deque<int>();

			for (int i = 0; i < uiChannels; i++)
			{
				UIChannelPool.push_back(i);
			}

			for (int i = 0; i < entityChannels; i++)
			{
				EntityChannelPool.push_back(i + uiChannels);
			}

			MusicSourcePool = 1;

			Mix_ChannelFinished(ReturnChannelAutomatically);

			previousUIVolumes.reserve(uiChannelCount);
			previousEntityVolumes.reserve(entityChannelCount);

			Muted = false;
		}
		

		static void Mute(AudioType audioType);
		static void Unmute(AudioType audioType);
		static void SetVolume(AudioType audioType, int volume);

		static void StopAll();
		static void PauseAll();
		static void UnpauseAll();

		// Returns a channel of the type that was asked.
		// If a -2 is returned, this implies there were no channels available
		// -1 in SDL_mixer implies any channel, which we don't want
		static int GetChannel(AudioType audioType);
		static void ReturnChannel(int channel, AudioType audioType);

		static void PlaySoundEffect();
		static void PlayMusic();


		static void Cleanup()
		{
		}




	private:
		AudioManager() {};

		static std::uint8_t uiChannelCount;
		static std::uint8_t entityChannelCount;

		static std::deque<int> UIChannelPool;
		static std::deque<int> EntityChannelPool;
		static int MusicSourcePool; // This might actually become a bool, since you can only have one song playing

		static std::uint8_t sourceCount;
		static bool Muted;


		static void ReturnChannelAutomatically(int channel);

		static std::vector<int> previousUIVolumes;
		static std::vector<int> previousEntityVolumes;

	};
}