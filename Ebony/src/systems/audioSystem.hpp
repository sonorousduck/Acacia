#pragma once

#include "system.hpp"
//#include "../components/audioSource.hpp"
//#include "../Audio/src/SoundDevice.hpp"
//#include "../Audio/src/MusicSource.hpp"
#include "../components/soundEffectComponent.hpp"
#include "../components/musicComponent.hpp"

namespace systems
{
	class AudioSystem : public System
	{
	public:
		AudioSystem() : System({ctti::unnamed_type_id<components::SoundEffect>(), ctti::unnamed_type_id<components::Music>() })
		{
			// Initialize for playing. This has to happen before it is able to play anything, so this seems like a great place!
			//SoundDevice::init();
			//EbonyAudio::Music::Init();
		}

		bool isInterested(const entities::EntityPtr& entity) override;

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
		
	};
}