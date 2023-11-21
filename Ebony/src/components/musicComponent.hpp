#pragma once

#include <deque>

#include "component.hpp"
#include "../Audio/src/SoundSource.hpp"

namespace components
{
	// This makes them so they can be byte comparisons


	class Music : public PolymorphicComparable<Component, Music>
	{
	public:
		Music() :
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
		bool repeat;

	};



}


