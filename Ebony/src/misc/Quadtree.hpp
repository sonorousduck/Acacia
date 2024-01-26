#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>
#include <unordered_set>


#include "../entity.hpp"

namespace Ebony
{
	class Quadtree
	{
	public:
		Quadtree(float x, float y, bool isStaticTree) : position(glm::vec2(0.0f)), currentLevel(0), isStaticQuadtree(isStaticTree), shouldRebuild(true) // Set rebuild to true, though if it is not static, this won't do anything
		{
			this->size = glm::vec2(x, y);
			children.reserve(4);
		};

		Quadtree(glm::vec2 position, glm::vec2 size, std::uint16_t currentLevel, bool isStaticTree) :
			position(position),
			size(size),
			currentLevel(currentLevel),
			isStaticQuadtree(isStaticTree),
			shouldRebuild(true) // Set true for the first time
		{
			children.reserve(4);
		}


		// Number of rigidbodies in each level. Once it exceeds this amount, it will split
		std::uint16_t MAX_OBJECTS_IN_LEVEL = 50;

		// Max number of levels that are allowed in the Quadtree
		// This is extremely important if the objects are completely overlapping
		// since that would cause an infinite loop (if MAX_OBJECTS_IN_LEVEL were overlapping)
		// If you set this value to 1, it is the same as if you just didn't have a quadtree
		std::uint16_t MAX_LEVELS = 100;

		// This eventually needs to change how it works, as it could be more eloquent
		std::uint16_t QUADTREE_SIZE = 10000; // This is the physics dimension height multiplied by 10
		std::uint16_t QUADTREE_MIDPOINT = QUADTREE_SIZE / 2;


		glm::vec2 position;
		glm::vec2 size;

		bool isStaticQuadtree;
		bool shouldRebuild;


		std::vector<bool> GetQuadrants(entities::EntityPtr entity);
		void Insert(entities::EntityPtr entity);
		void Split();
		
		
		std::vector<entities::EntityPtr> GetPossibleCollisions(entities::EntityPtr entity);

		void Clear();

	private:
		std::uint16_t currentLevel;
		std::vector<entities::EntityPtr> entitiesInLevel{};
		std::vector<Quadtree> children{};

		void GetPossibleCollisions(entities::EntityPtr entity, std::unordered_set<entities::EntityPtr>& results);


	};


}