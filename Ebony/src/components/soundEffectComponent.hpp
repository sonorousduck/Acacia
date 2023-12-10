#pragma once

#include <deque>

#include "component.hpp"
#include "../Audio/src/SoundSource.hpp"
#include <SDL_mixer.h>

namespace components
{
	// This makes them so they can be byte comparisons

	class SoundEffect : public PolymorphicComparable<Component, SoundEffect>
	{
	public:
		SoundEffect(Ebony::AudioType soundEffectType) : soundEffectType(soundEffectType)
		{

		}

		std::deque<Mix_Chunk*> soundEffectQueue{};
		Ebony::AudioType soundEffectType;

		bool fadesIn = false;
		bool fadesOut = false;

		int fadeInTime = 10; // ms
		int fadeOutTime = 10; // ms

		int volume = MIX_MAX_VOLUME;

	};



}


