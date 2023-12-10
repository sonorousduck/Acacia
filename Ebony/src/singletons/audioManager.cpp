#include "audioManager.hpp"


namespace Ebony
{
	void AudioManager::StopAll()
	{
		Mix_HaltChannel(-1);
		Mix_HaltMusic();
	}

	void AudioManager::PauseAll()
	{
		Mix_Pause(-1);
		Mix_PauseMusic();
	}

	void AudioManager::UnpauseAll()
	{
		Mix_Resume(-1);
		Mix_ResumeMusic();
	}

	void AudioManager::Mute(AudioType audioType)
	{
		if (audioType & UI)
		{
			for (int i = 0; i < uiChannelCount; i++)
			{
				previousUIVolumes[i] = Mix_Volume(i, 0);
			}
		}
		else if (audioType & ENTITY)
		{
			for (int i = 0; i < entityChannelCount; i++)
			{
				previousEntityVolumes[i] = Mix_Volume(i + uiChannelCount, 0);
			}
		}

		Muted = true;
	}

	void AudioManager::Unmute(AudioType audioType)
	{
		if (Muted)
		{
			if (audioType & UI)
			{
				for (int i = 0; i < uiChannelCount; i++)
				{
					Mix_Volume(i, previousUIVolumes[i]);
				}
			}
			else if (audioType & ENTITY)
			{
				for (int i = 0; i < entityChannelCount; i++)
				{
					Mix_Volume(i + uiChannelCount, previousEntityVolumes[i]);
				}
			}
			Muted = false;
		}
		else
		{
			std::cerr << "Unmute was called when the audio was not muted for the audio type: " << audioType << std::endl;
		}
	}



	void AudioManager::SetVolume(AudioType audioType, int volume)
	{
		if (audioType & UI)
		{
			for (int i = 0; i < uiChannelCount; i++)
			{
				Mix_Volume(i, volume);
			}
		}
		else if (audioType & ENTITY)
		{
			for (int i = 0; i < entityChannelCount; i++)
			{
				Mix_Volume(i + uiChannelCount, volume);
			}
		}
	}

	int AudioManager::GetChannel(AudioType audioType)
	{
		if (audioType & UI)
		{
			if (!UIChannelPool.empty())
			{
				int channel = UIChannelPool.front();
				UIChannelPool.pop_front();

				return channel;
			}
			
		}
		else if (audioType & ENTITY)
		{
			if (!EntityChannelPool.empty())
			{
				int channel = EntityChannelPool.front();
				EntityChannelPool.pop_front();

				return channel;
			}			
		}

		std::cerr << "Not enough channels in the given audio type: " << audioType << std::endl;
		return -2;
	}

	void AudioManager::ReturnChannel(int channel, AudioType audioType)
	{
		if (audioType & UI)
		{
			UIChannelPool.push_back(channel);
		}
		else if (audioType & ENTITY)
		{
			EntityChannelPool.push_back(channel);
		}
	}

	void AudioManager::PlaySoundEffect()
	{

	}

	void AudioManager::PlayMusic()
	{

	}

	void AudioManager::ReturnChannelAutomatically(int channel)
	{
		if (channel < uiChannelCount)
		{
			AudioManager::UIChannelPool.push_back(channel);
		}
		else
		{
			AudioManager::EntityChannelPool.push_back(channel);
		}
	}

}

