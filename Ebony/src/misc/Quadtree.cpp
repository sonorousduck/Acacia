#include "Quadtree.hpp"
#include "../components/collider.hpp"
#include "../components/transform.hpp"
#include <iostream>

namespace Ebony
{

	// Returns the quadrants in which this particular entity is located
	std::vector<bool> Quadtree::GetQuadrants(entities::EntityPtr entity)
	{
		// Assume that it has both a transform and a collider. The physics system shouldn't have updated it for collisions if this was not the case
		auto collider = entity->getComponent<components::Collider>();
		auto transform = entity->getComponent<components::Transform>();


		std::vector<bool> quadrants{};
		quadrants.resize(4);

		for (std::uint8_t i = 0; i < 4; i++)
		{
			if (children.size() - 1 < i || children.size() == 0)
			{
				quadrants.at(i) = false;
				continue;
			}

			
			// This will always be a rectangle since the Quadtree is meant to check for AABB collisions only. Other collision detection will be handled
			// by a more refined system. Quadtrees wouldn't help nor speed up that process at all
		
			// Adapted from Dr. Mathias' lecture slides

			glm::vec2 position = transform->position;
			glm::vec2 colliderSize = collider->aabbCollider.getSize();
			
			quadrants[i] = !(position.x + QUADTREE_MIDPOINT - colliderSize.x / 2.0f > children[i].position.x + children[i].size.x ||
							 position.x + QUADTREE_MIDPOINT - colliderSize.x / 2.0f < children[i].position.x ||
							 position.y + QUADTREE_MIDPOINT - colliderSize.y / 2.0f > children[i].position.y + children[i].size.y ||
							 position.y + QUADTREE_MIDPOINT + colliderSize.y / 2.0f < children[i].position.y);

		}



		return quadrants;
	}

	void Quadtree::Insert(entities::EntityPtr entity)
	{
		std::vector<bool> quadrantsToInsert = GetQuadrants(entity);

		// No children yet, we should just insert this
		if (children.size() == 0)
		{
			entitiesInLevel.push_back(entity);

			if (entitiesInLevel.size() > MAX_OBJECTS_IN_LEVEL && currentLevel < MAX_LEVELS)
			{
				Split();
			}
		}
		// Insert into each child to which it belongs
		else
		{
			for (std::uint8_t i = 0; i < 4; i++)
			{
				if (quadrantsToInsert[i])
				{
					children[i].Insert(entity);
				}
			}
		}
	}

	void Quadtree::Split()
	{
		children.push_back(Quadtree(position, size / 2.0f, currentLevel + 1));
		children.push_back(Quadtree(glm::vec2(position.x + size.x / 2.0f, position.y), size / 2.0f, currentLevel + 1));
		children.push_back(Quadtree(glm::vec2(position.x, position.y + size.y / 2.0f), size / 2.0f, currentLevel + 1));
		children.push_back(Quadtree(position + size / 2.0f, size / 2.0f, currentLevel + 1));


		for (auto i = 0; i < entitiesInLevel.size(); i++)
		{
			std::vector<bool> quadrants = GetQuadrants(entitiesInLevel[i]);

			for (std::uint8_t i = 0; i < 4; i++)
			{
				if (quadrants[i])
				{
					children[i].Insert(entitiesInLevel[i]);
				}
			}
		}
	}

	std::vector<entities::EntityPtr> Quadtree::GetPossibleCollisions(entities::EntityPtr entity)
	{
		if (children.size() == 0)
		{
			return entitiesInLevel;
		}

		std::unordered_set<entities::EntityPtr> results{};

		GetPossibleCollisions(entity, results);

		// Extract the information from the set and convert it to a std::vector. This might
		// be a region for potential speed up in the future
		// https://stackoverflow.com/questions/42519867/efficiently-moving-contents-of-stdunordered-set-to-stdvector
		std::vector<entities::EntityPtr> vectorResults{};
		vectorResults.reserve(results.size());
		for (auto it = results.begin(); it != results.end(); )
		{
			vectorResults.push_back(std::move(results.extract(it++).value()));
		}

		return vectorResults;
	}


	void Quadtree::Clear()
	{
		if (children.size() > 0)
		{
			for (std::uint8_t i = 0; i < 4; i++)
			{
				children[i].Clear();
			}
		}

		entitiesInLevel.clear();
		children.clear();
	}

	void Quadtree::GetPossibleCollisions(entities::EntityPtr entity, std::unordered_set<entities::EntityPtr>& results)
	{
		if (children.size() == 0)
		{
			// Union the results with all entities in this level
			results.insert(entitiesInLevel.begin(), entitiesInLevel.end());
		}

		std::vector<bool> possibleRegions = GetQuadrants(entity);

		// Add the possible collisions for each child
		for (std::uint8_t i = 0; i < 4; i++)
		{
			if (possibleRegions[i])
			{
				children[i].GetPossibleCollisions(entity, results);
			}
		}


	}
}
