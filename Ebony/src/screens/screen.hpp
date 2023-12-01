#pragma once

#include <cstdint>
#include <chrono>
#include <unordered_set>
#include <entity.hpp>
#include <graphics2d.hpp>

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
		virtual void Init(int windowWidth, int windowHeight) {}
		virtual void AddNewEntities() {}
		virtual void RemoveOldEntities() {}


		virtual void OnScreenFocus() {}
		virtual void OnScreenDefocus() {}

		virtual void AddEntity(entities::EntityPtr entity) {}
		virtual void RemoveEntity(entities::Entity::IdType id) {}

		

		// Returns the next screen
		virtual std::uint16_t Update(std::chrono::microseconds elapsedTime) { return 0; }
		virtual void Draw(std::chrono::microseconds elapsedTime) {}
		virtual void ProcessInput(std::chrono::microseconds elapsedTime) {}
	
	
		entities::EntityMap allEntities;
		std::vector<entities::EntityPtr> newEntities;
		std::unordered_set<entities::Entity::IdType> removeEntities;
		std::unordered_set<entities::Entity::IdType> updatedEntities;
		std::mutex mutexEntities; // This is necessary so we can add to the newEntities vector even though it is multithreaded

		std::uint64_t screen = 0;
		std::uint64_t nextScreen = 0;

		int windowWidth = 800;
		int windowHeight = 640;
	
		float deltaTime = 0.0f;
		float lastFrame = 0.0f;

	};
}


