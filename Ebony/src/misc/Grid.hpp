#pragma once

#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_set>


#include "../entity.hpp"


namespace Ebony
{
	class Grid
	{
	public:

		Grid(glm::vec2 position, glm::vec2 size, glm::vec2 sizeOfGridUnit, bool isStatic) : position(position), size(size), sizeOfGridUnit(sizeOfGridUnit), isStatic(isStatic), shouldRebuild(true)
		{
			gridAmount = static_cast<int>((size.x / sizeOfGridUnit.x) * (size.y / sizeOfGridUnit.y));
			amountPerRow = static_cast<int>(size.x / sizeOfGridUnit.x);
			grid.resize(gridAmount);
		}

		void Insert(entities::EntityPtr entity);
		
		std::vector<entities::EntityPtr> GetPossibleCollisions(entities::EntityPtr entity);
		void Clear();


		glm::vec2 position;
		int amountPerRow;
		int gridAmount;
		glm::vec2 size;
		glm::vec2 sizeOfGridUnit;

		bool isStatic;
		bool shouldRebuild;

	private:
		std::vector<std::vector<entities::EntityPtr>> grid{};

		void GetPossibleCollisions(entities::EntityPtr entity, std::unordered_set<entities::EntityPtr>& results);
	};
}