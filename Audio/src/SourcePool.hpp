#pragma once

#include "AudioType.hpp"
#include "SoundSource.hpp"
#include <vector>

namespace EbonyAudio
{
	class SourcePool
	{
	public:
		SourcePool(AudioType audioType, std::uint16_t sourceCount);

		// Requests a source from the source list. May return nothing if no sources available... (TODO: Implement this)
		std::unique_ptr<SoundSource> GetSource();

		void Init(AudioType type);

		// Returns a source back to the source list
		void ReleaseSource(std::unique_ptr<SoundSource> source);
		
		// i.e. if you want to have another new source added to the source list to pull from
		void AddNewSource(std::unique_ptr<SoundSource> source);

		// i.e. removes a potential source from the list, shrinking the number of available sources by one
		void DeleteSourcePermanently();


		// Should be called to delete all sources that are tracked.
		void CleanUp()
		{
			std::cout << "Cleaning up sources" << std::endl;
			for (auto i = 0; i < sources.size(); i++)
			{
				sources[i]->Delete();
			}

			sources.clear();
		}


		size_t GetSize()
		{
			return this->sources.size();
		}

		std::uint16_t sourceCount;


	private:

		// Holds all the available sources that can be pulled form
		std::vector<std::unique_ptr<SoundSource>> sources{};

		// Specifies the type of pool this is. Mostly useful for the Audio Manager that is managing this pool
		AudioType poolType;


	};
}