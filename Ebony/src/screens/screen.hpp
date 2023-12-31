#pragma once

#include "../entity.hpp"
#include <cstdint>
#include <chrono>
#include <unordered_set>
#include <graphics2d.hpp>
#include <mutex>

namespace Ebony
{
	// This will be implemented by other games mostly, just not DEFAULT of 0. This is to let the 
	// screen declare the ScreenEnum type (Potentially circle back and just make this a std::uint16_t instead of the ScreenEnum)
	enum ScreenEnum : std::uint64_t
	{
		DEFAULT = 0,
	};


	class Screen
	{
	public:
		virtual void LoadContent() {}
		//virtual void Run();
		virtual void Start() {};
		virtual void Init(int windowWidth, int windowHeight) {}
		virtual void AddNewEntities() {}
		virtual void RemoveOldEntities() {}


		// This way, you can control if they are coming from the pause menu to not restart your game, but if they are
		// coming from the main menu screen, to set everything up
		virtual void OnScreenFocus(std::uint64_t lastScreenEnum) {}

		// Now you can tell which screen they are going to. That way you can cleanup depending on 
		// if they are just pausing, or changing to a new scene entirely
		virtual void OnScreenDefocus(std::uint64_t lastScreenEnum) {}

		virtual void AddEntity(entities::EntityPtr entity) {}
		virtual void RemoveEntity(entities::Entity::IdType id) {}

		

		// Returns the next screen
		virtual std::uint64_t Update(std::chrono::microseconds elapsedTime) { return 0; }
		virtual void Draw(std::chrono::microseconds elapsedTime) {}
		virtual void ProcessInput(std::chrono::microseconds elapsedTime) {}
	
	
		entities::EntityMap allEntities;
		std::vector<entities::EntityPtr> newEntities;
		std::unordered_set<entities::Entity::IdType> removeEntities;
		std::unordered_set<entities::Entity::IdType> updatedEntities;
		std::mutex mutexEntities; // This is necessary so we can add to the newEntities vector even though it is multithreaded

		std::uint64_t screen;
		std::uint64_t nextScreen;

		int windowWidth = 800;
		int windowHeight = 640;
	
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;
		bool alreadyStarted = false;

	};
}


