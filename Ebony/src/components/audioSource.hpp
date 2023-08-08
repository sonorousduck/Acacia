#pragma once

#include <deque>

#include "component.hpp"
#include "../Audio/src/MusicSource.hpp"
#include "../Audio/src/SoundSource.hpp"

namespace components
{
	// This makes them so they can be byte comparisons
	enum State
	{
		Stopped = 1,
		Playing = 2,
		Paused = 4,
	};

	class AudioSource : public PolymorphicComparable<Component, AudioSource>
	{
	public:
		AudioSource() :
			previousState(Stopped),
			currentState(Stopped),
			previousSong(""),
			currentSong(""),
			repeat(false)
		{}

		State previousState;
		State currentState;

		std::string_view previousSong;
		std::string_view currentSong;
		EbonyAudio::MusicSource source;

		bool repeat;

		std::deque<std::string_view> soundEffectQueue{};

		// This should contain a few speakers (that can grow) so you can just reuse them instead of creating new ones everytime.
		// Maybe just contain one in the beginning, but if it ever needs more sound sources, it can expand
		std::vector<SoundSource> soundSources{SoundSource()};

		std::uint16_t useableSoundIndex = 0;


	};


}