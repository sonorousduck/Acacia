#include "audioSystem.hpp"


namespace systems
{
	void AudioSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void AudioSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto audioSource = entity->getComponent<components::AudioSource>();

			// If current song is playing, we need to update the buffers
			if (audioSource->currentState & components::Playing)
			{
				audioSource->source.UpdateBufferStream();
			}
			
			// TODO: Have some check to see if the source has completed playing

			// After updating the buffers, if there are any sound effects in the queue, we want to play them.
			while (!audioSource->soundEffectQueue.empty())
			{
				auto& soundSources = audioSource->soundSources;
				
				// First, check if the usable index is usable for playing a sound
				if (!soundSources.at(audioSource->useableSoundIndex).isPlaying)
				{
					// Queue the song
					// Increment the index

					std::string_view test = audioSource->soundEffectQueue.front();
					audioSource->soundEffectQueue.pop_front();

					//soundSources.at(audioSource->useableSoundIndex).Play(); 
					audioSource->useableSoundIndex = (static_cast<unsigned long long>(audioSource->useableSoundIndex) + 1) % soundSources.size();

				}
				else
				{
					bool foundIndex = false;
					for (auto i = audioSource->useableSoundIndex + 1; i < soundSources.size(); i++)
					{
						if (!soundSources.at(i).isPlaying)
						{
							// Queue the song
							//soundSources.at(i).Play(audioSource->soundEffectQueue.pop_front());
							// Set the index
							audioSource->useableSoundIndex = static_cast<std::uint16_t>((static_cast<unsigned long long>(i) + 1) % soundSources.size());


							foundIndex = true;
							break;
						}
					}

					if (!foundIndex)
					{
						for (std::uint16_t i = 0; i < audioSource->useableSoundIndex; i++)
						{
							if (!soundSources.at(i).isPlaying)
							{
								// Queue the song
								// Set the index
								//soundSources.at(i).Play(audioSource->soundEffectQueue.pop_front());
								audioSource->useableSoundIndex = static_cast<std::uint16_t>((static_cast<unsigned long long>(i) + 1) % soundSources.size());


								foundIndex = true;
								break;
							}
						}
					}

					// If still no index, create new ones and use that
					if (!foundIndex)
					{
						soundSources.emplace_back(SoundSource());
						soundSources.emplace_back(SoundSource());


						// Queue the song
						// Set the index
						//soundSources.at(soundSources.size() - 2).Play(audioSource->soundEffectQueue.pop_front());
						audioSource->useableSoundIndex = static_cast<std::uint16_t>(soundSources.size() - 1);


					}
				}
			}

		}


	}




}
