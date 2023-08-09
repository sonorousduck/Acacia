#pragma once

#include "system.hpp"
#include "../components/audioSource.hpp"
#include "../Audio/src/SoundDevice.hpp"
#include "../Audio/src/MusicSource.hpp"

namespace systems
{
	class AudioSystem : public System
	{
	public:
		AudioSystem() : System({ctti::unnamed_type_id<components::AudioSource>()})
		{
			// Initialize for playing. This has to happen before it is able to play anything, so this seems like a great place!
			//SoundDevice::init();
			//EbonyAudio::Music::Init();
		}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
		
	};
}