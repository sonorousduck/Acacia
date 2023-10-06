#pragma once
#include <cstdint>
#include <vector>
#include <glm/glm.hpp>


#include "../entity.hpp"

namespace Ebony
{
	class Quadtree
	{
	public:
		Quadtree(float x, float y) : position(glm::vec2(0.0f)), currentLevel(0) 
		{
			this->size = glm::vec2(x, y);
			children.resize(4);
		};

		Quadtree(glm::vec2 position, glm::vec2 size, std::uint16_t currentLevel) :
			position(position),
			size(size),
			currentLevel(currentLevel)
		{
			children.resize(4);
		}


		// Number of rigidbodies in each level. Once it exceeds this amount, it will split
		std::uint16_t MAX_OBJECTS_IN_LEVEL = 20;

		// Max number of levels that are allowed in the Quadtree
		// This is extremely important if the objects are completely overlapping
		// since that would cause an infinite loop (if MAX_OBJECTS_IN_LEVEL were overlapping)
		// If you set this value to 1, it is the same as if you just didn't have a quadtree
		std::uint16_t MAX_LEVELS = 200;

		glm::vec2 position;
		glm::vec2 size;


		std::vector<bool> GetQuadrants(entities::EntityPtr entity);
		void Insert(entities::EntityPtr entity);
		void Split();
		
		
		std::vector<entities::EntityPtr> GetPossibleCollisions(entities::EntityPtr entity);




	private:
		std::uint16_t currentLevel;
		std::vector<entities::EntityPtr> entitiesInLevel{};
		std::vector<Quadtree> children{};


	};


}