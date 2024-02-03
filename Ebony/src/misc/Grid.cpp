#include "Grid.hpp"
#include "../components/collider.hpp"
#include "../components/transform.hpp"
#include <iostream>


namespace Ebony
{
	void Grid::Insert(entities::EntityPtr entity)
	{
		// We need to get the position of the entity, the overall size
		auto transform = entity->getComponent<components::Transform>();
		auto collider = entity->getComponent<components::Collider>();

		glm::vec2 colliderSize = collider->aabbCollider.getSize();
		glm::vec2 centerOfCollider = collider->aabbCollider.getCenter();
		glm::vec2 position = transform->position + centerOfCollider - (0.5f * colliderSize);
		glm::vec2 endPosition = position + colliderSize;

		// We know our grid size as well as our individual grid unit sizes
		auto startColumn = static_cast<int>(position.x / sizeOfGridUnit.x);
		auto endColumn = static_cast<int>(endPosition.x / sizeOfGridUnit.x);
		auto startRow = static_cast<int>(position.y / sizeOfGridUnit.y);
		auto endRow = static_cast<int>(endPosition.y / sizeOfGridUnit.y);

		for (auto y = startRow; y <= endRow; y++)
		{
			for (auto x = startColumn; x <= endColumn; x++)
			{
				if (amountPerRow * y + x < grid.size())
				{
					grid[amountPerRow * y + x].push_back(entity);
				}
				else
				{
					std::cout << "Caution! Out of the physics domain" << std::endl;
				}
			}
		}
	}

	void Grid::Clear()
	{
		grid.clear();
		grid.resize(static_cast<int>(gridAmount));

	}

	std::vector<entities::EntityPtr> Grid::GetPossibleCollisions(entities::EntityPtr entity)
	{
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

	void Grid::GetPossibleCollisions(entities::EntityPtr entity, std::unordered_set<entities::EntityPtr>& results)
	{
		auto transform = entity->getComponent<components::Transform>();
		auto collider = entity->getComponent<components::Collider>();

		glm::vec2 colliderSize = collider->aabbCollider.getSize();
		glm::vec2 centerOfCollider = collider->aabbCollider.getCenter();
		glm::vec2 startPosition = transform->position + centerOfCollider - (0.5f * colliderSize);
		glm::vec2 endPosition = startPosition + colliderSize;

		// We know our grid size as well as our individual grid unit sizes
		auto startColumn = static_cast<int>(startPosition.x / sizeOfGridUnit.x);
		auto endColumn = static_cast<int>(endPosition.x / sizeOfGridUnit.x);
		auto startRow = static_cast<int>(startPosition.y / sizeOfGridUnit.y);
		auto endRow = static_cast<int>(endPosition.y / sizeOfGridUnit.y);

		auto entityLayer = entity->getComponent<components::Collider>()->layersToCollideWith;



		for (auto y = startRow; y <= endRow; y++)
		{
			for (auto x = startColumn; x <= endColumn; x++)
			{
				if (amountPerRow * y + x < 0 || (amountPerRow * y + x) > grid.size())
				{
					return;
				}

				auto indexPosition = amountPerRow * y + x;

				for (auto i = 0; i < grid[indexPosition].size(); i++)
				{
					auto& possibleCollision = grid[indexPosition][i];

					if (possibleCollision->getComponent<components::Collider>()->layer & entityLayer && possibleCollision != entity)
					{
						results.insert(possibleCollision);
					}
				}
			}
		}



	}
}

