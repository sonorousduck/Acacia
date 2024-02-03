#pragma once
#include "system.hpp"
#include "../components/collider.hpp"
#include "../components/rigidbodyComponent.hpp"
#include "../misc/Grid.hpp"
#include "../components/transform.hpp"
#include "../misc/ThreadPool.hpp"
#include "../components/cppScriptComponent.hpp"
#include <latch>
#include <algorithm>

#include <glm/glm.hpp>


namespace systems
{
	class PhysicsSystem : public System
	{
	public:
		PhysicsSystem() : System({ ctti::unnamed_type_id<components::Collider>(), ctti::unnamed_type_id<components::RigidBody>(), ctti::unnamed_type_id<components::Transform>() }),
			dimensions(glm::vec2(480, 320)),
			gridSize(glm::vec2(16, 16)),
			gridStartPosition(glm::vec2(-10.0f, -10.0f))

		{
		}

		PhysicsSystem(glm::vec2 dimensions, glm::vec2 gridSize) : System({ ctti::unnamed_type_id<components::Collider>(), ctti::unnamed_type_id<components::RigidBody>(), ctti::unnamed_type_id<components::Transform>()}), 
			dimensions(dimensions), 
			gridSize(gridSize),
			gridStartPosition(glm::vec2(-10.0f, -10.0f))
		{
		}

		PhysicsSystem(glm::vec2 dimensions, glm::vec2 gridSize, glm::vec2 gridStartPos) : System({ ctti::unnamed_type_id<components::Collider>(), ctti::unnamed_type_id<components::RigidBody>(), ctti::unnamed_type_id<components::Transform>() }),
			dimensions(dimensions),
			gridSize(gridSize),
			gridStartPosition(gridStartPos)
		{


		}

		void Update(std::chrono::microseconds elapsedTime) override;

		void RemoveEntity(entities::Entity::IdType entityId) override;

		glm::vec2 dimensions;
		glm::vec2 gridSize;
		glm::vec2 gridStartPosition;
	protected:
		virtual bool isInterested(const entities::EntityPtr& entity);

		void updateImpl(std::chrono::microseconds elapsedTime);


		
	private:
		/*std::uint16_t PHYSICS_DIMENSION_WIDTH = 1000;
		std::uint16_t PHYSICS_DIMENSION_HEIGHT = 1000;*/
		float GRAVITY_CONSTANT = 9810.0f;
		/*std::uint16_t QUADTREE_SIZE = PHYSICS_DIMENSION_HEIGHT * 10;
		std::uint16_t QUADTREE_MIDPOINT = QUADTREE_SIZE / 2;*/

		/*Ebony::Quadtree quadtree = Ebony::Quadtree(QUADTREE_SIZE, QUADTREE_SIZE, false);
		Ebony::Quadtree staticQuadtree = Ebony::Quadtree(QUADTREE_SIZE, QUADTREE_SIZE, true);*/

		Ebony::Grid grid = Ebony::Grid(gridStartPosition, dimensions, gridSize, false);
		Ebony::Grid staticGrid = Ebony::Grid(gridStartPosition, dimensions, gridSize, true);


		
		void updateObject(std::chrono::microseconds elapsedTime, entities::EntityPtr entity);


		bool HasCollision(const entities::EntityPtr entity, const entities::EntityPtr otherEntity);
		bool HasAABBCollision(const entities::EntityPtr& entity, const entities::EntityPtr& otherEntity);
	};
}