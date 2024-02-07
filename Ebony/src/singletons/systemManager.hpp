#pragma once

#include "../entity.hpp"
#include "../screens/screen.hpp"

namespace Ebony
{
	class SystemManager
	{

	public:
		static void AddEntity(entities::EntityPtr entity);
		static void RemoveEntity(entities::Entity::IdType id);
		static void AddNewEntities();
		static void RemoveOldEntities();



		static bool newScreenFocused;
		static bool quit;
		static std::unordered_map<std::uint64_t, std::shared_ptr<Screen>> screens;
		static std::shared_ptr<Screen> currentScreen;
		static std::uint64_t nextScreenEnum;
		static std::uint64_t currentScreenEnum;
		static std::uint64_t lastScreenEnum;

		static bool aiEnabled;
		static bool shouldResetForAi;
		static bool goodEnding;

	private:
		SystemManager() {}




	};
}