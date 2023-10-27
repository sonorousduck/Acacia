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
		{
		}

		State previousState;
		State currentState;

		std::string_view previousSong;
		std::string_view currentSong;
		std::shared_ptr<EbonyAudio::MusicSource> source;
		std::deque<ALuint> soundEffectQueue{};

		bool repeat;
		std::uint16_t useableSoundIndex = 0;


	};


}