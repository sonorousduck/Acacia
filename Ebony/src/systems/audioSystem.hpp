#pragma once

#include "system.hpp"
#include "../components/audioSource.hpp"

namespace systems
{
	class AudioSystem : public System
	{
	public:
		AudioSystem() : System({ctti::unnamed_type_id<components::AudioSource>()})
		{}

		void Update(std::chrono::microseconds elapsedTime) override;

	private:
		void updateImpl(std::chrono::microseconds elapsedTime);
		
	};
}