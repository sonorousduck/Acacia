#pragma once
#include <entity.hpp>
#include <misc/resourceManager.hpp>
#include <components/musicComponent.hpp>


namespace Crypt
{
	class MainMusicPrefab
	{
	public:
		static entities::EntityPtr Create(const char* musicFilename, int startingVolumeLevel)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto music = std::make_unique<components::Music>(Ebony::IndividualMusicTrack(Ebony::ResourceManager::GetMusic(musicFilename), startingVolumeLevel));

			entity->addComponent(std::move(music));

			return entity;
		}
	};
}