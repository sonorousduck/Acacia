#include "audioSystem.hpp"
#include <algorithm>
#include "../singletons/audioManager.hpp"




namespace systems
{
	bool AudioSystem::isInterested(const entities::EntityPtr& entity)
	{
		bool doICare = std::any_of(
			m_Interests.begin(), m_Interests.end(),
			[&entity](auto interest)
			{
				return entity->getComponents().contains(interest);
			});


		return doICare;
	}
	void AudioSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AudioSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			// Handle Sound Effects
			if (entity->hasComponent<components::SoundEffect>())
			{
				auto soundEffect = entity->getComponent<components::SoundEffect>();

				while (!soundEffect->soundEffectQueue.empty())
				{
					int channel = Ebony::AudioManager::GetChannel(soundEffect->soundEffectType);

					auto& nextSoundEffect = soundEffect->soundEffectQueue.front();

					if (channel != -2)
					{
						Mix_Volume(channel, nextSoundEffect.volume);
						if (nextSoundEffect.fadesIn)
						{
							Mix_FadeInChannel(channel, nextSoundEffect.soundEffect, 0, nextSoundEffect.fadeInTime);
						}
						else
						{
							
							Mix_PlayChannel(channel, nextSoundEffect.soundEffect, 0);
						}
						soundEffect->soundEffectQueue.pop_front();
					}
					else
					{
						// TODO: Skip for now. This means there weren't enough sounds to play it. Should think about how to handle this in the future
					}
				}
			}

			if (entity->hasComponent<components::Music>())
			{
				auto music = entity->getComponent<components::Music>();
				auto& song = music->songQueue.front();


				Mix_VolumeMusic(song.volume);

				if (music->currentState & Ebony::Stopped && !Mix_PlayingMusic() && !music->songQueue.empty())
				{
					music->songQueue.pop_front();

					if (song.fadesIn)
					{
						Mix_FadeInMusic(song.musicTrack, song.repeatTimes, song.fadeInTime);
					}
					else
					{
						Mix_PlayMusic(song.musicTrack, song.repeatTimes);
					}

					if (music->repeatPlaylist)
					{
						music->songQueue.push_back(song);
					}
				}
			}
		}
	}
}
