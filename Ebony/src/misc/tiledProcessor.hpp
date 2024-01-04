#pragma once

#include "tileson.hpp"
#include "glm/glm.hpp"
#include <iostream>
#include <functional>
#include "../entity.hpp"

namespace Ebony
{
	class TiledProcessor
	{
	public:
		TiledProcessor()
		{};



		// This should be called after AddEntityDefinitions has already been called
		virtual void ParseMap(const char* filepath, std::function<void(entities::EntityPtr)> AddEntity);

		// This function will be implemented by the developer in each game.
		virtual void CreateTranslationFunction();

		std::function<void(glm::vec2 position, int entityId)> translationFunction;


	private:

	};
}