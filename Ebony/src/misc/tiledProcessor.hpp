#pragma once
//#include "tileson.hpp"
//#include <cstdint>
//#include <vector>
//#include "glm/glm.hpp"
//#include "../Log.hpp"
#include <iostream>
#include <functional>


namespace Ebony
{
	class TiledProcessor
	{
	public:
		TiledProcessor()
		{};



		// This should be called after AddEntityDefinitions has already been called
		virtual void ParseMap(const char* filepath);

		// This function will be implemented by the developer in each game.
		virtual void CreateTranslationFunction();

		std::function<void(int entityId)> translationFunction{};


	private:

	};
}