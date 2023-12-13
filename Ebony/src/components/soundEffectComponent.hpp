#pragma once

#include <deque>

#include "component.hpp"
//#include "../Audio/src/SoundSource.hpp"
#include <SDL_mixer.h>
#include "../singletons/audioManager.hpp"

namespace Ebony
{
	class IndividualSound
	{
	public:
		IndividualSound(Mix_Chunk* soundEffect, int volume = MIX_MAX_VOLUME, bool fadesIn = false, bool fadesOut = false, int fadeInTime = 10, int fadeOutTime = 10) :
			soundEffect(soundEffect),
			volume(volume),
			fadesIn(fadesIn),
			fadesOut(fadesOut),
			fadeInTime(fadeInTime),
			fadeOutTime(fadeOutTime)
		{

		}

		Mix_Chunk* soundEffect;
		int volume;

		bool fadesIn;
		bool fadesOut;

		int fadeInTime; // ms
		int fadeOutTime; // ms
	};
}


namespace components
{
	// This makes them so they can be byte comparisons


	class SoundEffect : public PolymorphicComparable<Component, SoundEffect>
	{
	public:
		// TODO: CHANGE THIS BACK TO EBONY::AUDIOTYPE
		SoundEffect(Ebony::AudioType soundEffectType) : soundEffectType(soundEffectType)
		{
			
		}

		std::deque<Ebony::IndividualSound> soundEffectQueue{};
		Ebony::AudioType soundEffectType;
	};



}


