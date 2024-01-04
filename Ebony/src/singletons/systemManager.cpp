#include "systemManager.hpp"

namespace Ebony
{
	bool SystemManager::newScreenFocused{ false };
	bool SystemManager::quit{ false };


	std::shared_ptr<Screen> SystemManager::currentScreen{};
	std::unordered_map<std::uint64_t, std::shared_ptr<Screen>> SystemManager::screens{};

	std::uint64_t SystemManager::nextScreenEnum{ ScreenEnum::DEFAULT };
	std::uint64_t SystemManager::currentScreenEnum{ ScreenEnum::DEFAULT };
	std::uint64_t SystemManager::lastScreenEnum{ ScreenEnum::DEFAULT };


	void SystemManager::AddEntity(entities::EntityPtr entity)
	{
		currentScreen->AddEntity(entity);
	}

	void SystemManager::RemoveEntity(entities::Entity::IdType id)
	{
		currentScreen->RemoveEntity(id);
	}

	void SystemManager::AddNewEntities()
	{
		currentScreen->AddNewEntities();
	}

	void SystemManager::RemoveOldEntities()
	{
		currentScreen->RemoveOldEntities();
	}

}