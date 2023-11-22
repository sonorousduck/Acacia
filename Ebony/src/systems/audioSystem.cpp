#include "audioSystem.hpp"
#include <algorithm>
#include "../../../Audio/src/audioManager.hpp"


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
					auto effect = EbonyAudio::AudioManager::PlaySound(soundEffect->soundEffectQueue.back(), soundEffect->soundEffectType);
					soundEffect->soundEffectQueue.pop_back();
				}
			}

			if (entity->hasComponent<components::Music>())
			{
				auto music = entity->getComponent<components::Music>();

				if (music->musicSource->currentState & Stopped)
				{
					EbonyAudio::AudioManager::PlayMusic(music->musicSource);
				}
			}



			// Handle Music


			//auto audioSource = entity->getComponent<components::AudioSource>();

			// If current song is playing, we need to update the buffers
			//if (audioSource->currentState & components::Playing)
			//{
			//	audioSource->source.UpdateBufferStream();
			//}
			
			// TODO: Have some check to see if the source has completed playing

			// After updating the buffers, if there are any sound effects in the queue, we want to play them.
			//while (!audioSource->soundEffectQueue.empty())
			//{
			//	auto& soundSources = audioSource->soundSources;
			//	
			//	// First, check if the usable index is usable for playing a sound
			//	if (!soundSources.at(audioSource->useableSoundIndex).isPlaying)
			//	{
			//		// Queue the song
			//		// Increment the index

			//		ALuint soundEffect = audioSource->soundEffectQueue.front();
			//		audioSource->soundEffectQueue.pop_front();

			//		soundSources.at(audioSource->useableSoundIndex).Play(soundEffect);
			//		audioSource->useableSoundIndex = (static_cast<unsigned long long>(audioSource->useableSoundIndex) + 1) % soundSources.size();

			//	}
			//	else
			//	{
			//		bool foundIndex = false;
			//		for (auto i = audioSource->useableSoundIndex + 1; i < soundSources.size(); i++)
			//		{
			//			if (!soundSources.at(i).isPlaying)
			//			{
			//				// Queue the song
			//				ALuint soundEffect = audioSource->soundEffectQueue.front();
			//				audioSource->soundEffectQueue.pop_front();

			//				soundSources.at(i).Play(soundEffect);
			//				// Set the index
			//				audioSource->useableSoundIndex = static_cast<std::uint16_t>((static_cast<unsigned long long>(i) + 1) % soundSources.size());


			//				foundIndex = true;
			//				break;
			//			}
			//		}

			//		if (!foundIndex)
			//		{
			//			for (std::uint16_t i = 0; i < audioSource->useableSoundIndex; i++)
			//			{
			//				if (!soundSources.at(i).isPlaying)
			//				{
			//					// Queue the song
			//					// Set the index
			//					ALuint soundEffect = audioSource->soundEffectQueue.front();
			//					audioSource->soundEffectQueue.pop_front();

			//					soundSources.at(i).Play(soundEffect);
			//					//soundSources.at(i).Play(audioSource->soundEffectQueue.pop_front());
			//					audioSource->useableSoundIndex = static_cast<std::uint16_t>((static_cast<unsigned long long>(i) + 1) % soundSources.size());


			//					foundIndex = true;
			//					break;
			//				}
			//			}
			//		}

			//		// If still no index, create new ones and use that
			//		if (!foundIndex)
			//		{
			//			soundSources.emplace_back(SoundSource());
			//			soundSources.emplace_back(SoundSource());


			//			// Queue the song
			//			// Set the index
			//			ALuint soundEffect = audioSource->soundEffectQueue.front();
			//			audioSource->soundEffectQueue.pop_front();

			//			soundSources.at(soundSources.size() - 2).Play(soundEffect);
			//			//soundSources.at(soundSources.size() - 2).Play(audioSource->soundEffectQueue.pop_front());
			//			audioSource->useableSoundIndex = static_cast<std::uint16_t>(soundSources.size() - 1);


			//		}
			//	}
			//}

		}


	}




}
