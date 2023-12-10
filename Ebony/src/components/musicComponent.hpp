#pragma once

#include <deque>

#include "component.hpp"
#include "../Audio/src/SoundSource.hpp"
#include "../../../Audio/src/MusicSource.hpp"
#include <SDL_mixer.h>

namespace components
{
	// This makes them so they can be byte comparisons


	class Music : public PolymorphicComparable<Component, Music>
	{
	public:
		Music() :
			previousSong(""),
			currentSong(""),
			repeat(false)
		{}

		Music(Mix_Music* musicSource) :
			previousSong(""),
			currentSong(""),
			repeat(false),
			musicSource(musicSource)
		{}

		std::string_view previousSong;
		std::string_view currentSong;
		std::vector<std::string_view> songQueue = {};
		Mix_Music* musicSource = nullptr;
		
		bool repeat;
		int repeatTimes = 1;

		bool fadesIn = false;
		bool fadesOut = false;

		int fadeInTime = 10; // ms
		int fadeOutTime = 10; // ms

		Ebony::State currentState = Ebony::State::Stopped;
		Ebony::State previousState = Ebony::State::Stopped;



	};



}


