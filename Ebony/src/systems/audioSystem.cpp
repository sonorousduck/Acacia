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
			if (audioSource->currentState == components::Playing)
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
				}
				else
				{
					bool foundIndex = false;
					for (std::uint16_t i = audioSource->useableSoundIndex + 1; i < soundSources.size(); i++)
					{
						if (!soundSources.at(i).isPlaying)
						{
							// Queue the song
							// Set the index
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
					}
				}

				// If not, run a for loop check from index to the end

				// If still not, run a for loop from beginning to index

				// Else, add 2 audio sources to the vector
			}

		}


	}




}
