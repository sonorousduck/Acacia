#pragma once

#include <deque>

#include "component.hpp"
//#include "../Audio/src/SoundSource.hpp"
#//include "../../../Audio/src/MusicSource.hpp"
#include <SDL_mixer.h>

namespace Ebony
{
	class IndividualMusicTrack
	{
	public:
		IndividualMusicTrack(Mix_Music* musicTrack, int volume = MIX_MAX_VOLUME, bool fadesIn = false, bool fadesOut = false, int fadeInTime = 1000, int fadeOutTime = 1000, int repeatTimes = 0) :
			musicTrack(musicTrack),
			volume(volume),
			fadesIn(fadesIn),
			fadesOut(fadesOut),
			fadeInTime(fadeInTime),
			fadeOutTime(fadeOutTime),
			repeatTimes(repeatTimes)
		{

		}

		Mix_Music* musicTrack;
		int volume;

		bool fadesIn;
		bool fadesOut;

		int fadeInTime; // ms
		int fadeOutTime; // ms
		int repeatTimes;
	};
}

namespace components
{
	// This makes them so they can be byte comparisons


	class Music : public PolymorphicComparable<Component, Music>
	{
	public:
		Music() :
			previousSong(""),
			currentSong("")
		{}

		Music(Ebony::IndividualMusicTrack musicSource) :
			previousSong(""),
			currentSong("")
		{
			songQueue.push_back(musicSource);
		}

		Music(std::vector<Ebony::IndividualMusicTrack> musicSource) :
			previousSong(""),
			currentSong("")
		{

			for (int i = 0; i < musicSource.size(); i++)
			{
				songQueue.push_back(musicSource[i]);
			}
		}

		std::string_view previousSong;
		std::string_view currentSong;
		std::deque<Ebony::IndividualMusicTrack> songQueue = {};
		 /*musicSource = nullptr;*/
		
		Ebony::State currentState = Ebony::State::Stopped;
		Ebony::State previousState = Ebony::State::Stopped;

		bool repeatPlaylist = true;


	};



}


