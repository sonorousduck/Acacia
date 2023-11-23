#pragma once

#include <deque>

#include "component.hpp"
#include "../Audio/src/SoundSource.hpp"
#include "../../../Audio/src/MusicSource.hpp"

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

		Music(std::shared_ptr<EbonyAudio::MusicSource> musicSource) :
			previousSong(""),
			currentSong(""),
			repeat(false),
			musicSource(musicSource)
		{}

		std::string_view previousSong;
		std::string_view currentSong;
		std::vector<std::string_view> songQueue = {};
		std::shared_ptr<EbonyAudio::MusicSource> musicSource;
		bool repeat;

	};



}


