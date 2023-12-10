#include "audioSystem.hpp"
#include <algorithm>
//#include "../../../Audio/src/audioManager.hpp"
#include "../Ebony/src/singletons/audioManager.hpp"




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


					//auto effect = Ebony::AudioManager::PlaySoundEffect(soundEffect->soundEffectQueue.back(), soundEffect->soundEffectType);
					//soundEffect->soundEffectQueue.pop_back();
				}
			}

			if (entity->hasComponent<components::Music>())
			{
				auto music = entity->getComponent<components::Music>();

				if (music->currentState & Stopped && !Mix_PlayingMusic())
				{
					if (music->fadesIn)
					{
						Mix_FadeInMusic(music->musicSource, music->repeatTimes, music->fadeInTime);
					}
					else
					{
						Mix_PlayMusic(music->musicSource, music->repeatTimes);
					}
				}
			}
		}
	}
}
