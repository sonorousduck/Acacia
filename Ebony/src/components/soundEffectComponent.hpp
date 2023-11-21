#pragma once

#include <deque>

#include "component.hpp"
#include "../Audio/src/SoundSource.hpp"

namespace components
{
	// This makes them so they can be byte comparisons

	class SoundEffect : public PolymorphicComparable<Component, SoundEffect>
	{
	public:
		SoundEffect(EbonyAudio::AudioType soundEffectType) : soundEffectType(soundEffectType)
		{

		}

		std::deque<ALuint> soundEffectQueue{};
		EbonyAudio::AudioType soundEffectType;

	};



}


