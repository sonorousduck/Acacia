#pragma once

#include <optional>
#include <functional>
#include <glm/glm.hpp>
#include "../entity.hpp"

#include <stdexcept>

namespace components
{
	class Subcollider
	{
	public:
		Subcollider(glm::vec2 center, glm::vec2 size, bool isRect, bool isAABB) :
			center(center),
			size(size),
			isRect(isRect)
		{
			if (!isRect && isAABB)
			{
				throw std::invalid_argument("Circles can not be Axis Aligned Bounding Boxes (Implementation may change at a later point");
			}
			else
			{
				this->isAABB = isAABB;
			}
		
		};

		Subcollider(glm::vec2 center, 
			glm::vec2 size, 
			bool isRect,
			bool isAABB, 
			std::optional<std::function<void(entities::EntityPtr, std::chrono::microseconds elapsedTime)>> onCollisionStart,
			std::optional<std::function<void(entities::EntityPtr, std::chrono::microseconds elapsedTime)>> onCollision,
			std::optional<std::function<void(entities::EntityPtr, std::chrono::microseconds elapsedTime)>> onCollisionEnd
		) :
			center(center),
			size(size),
			isRect(isRect),
			onCollisionStart(onCollisionStart),
			onCollision(onCollision),
			onCollisionEnd(onCollisionEnd)
		{
			if (!isRect && isAABB)
			{
				throw std::invalid_argument("Circles can not be Axis Aligned Bounding Boxes (Implementation may change at a later point");
			}
			else
			{
				this->isAABB = isAABB;
			}
		};


		bool isRect;
		bool getIsAABB() { return this->isAABB; }
		bool isCollidingLastFrame = false;

		glm::vec2 lastCollisionLocation{};

		// Basically, the callback will take itself and the object with which it collided
		std::optional<std::function<void(entities::EntityPtr, std::chrono::microseconds elapsedTime)>> onCollisionStart;
		std::optional<std::function<void(entities::EntityPtr, std::chrono::microseconds elapsedTime)>> onCollision;
		std::optional<std::function<void(entities::EntityPtr, std::chrono::microseconds elapsedTime)>> onCollisionEnd;

		glm::vec2 getSize()
		{
			return this->size;
		}

		void changeSize(glm::vec2 newSize)
		{
			this->size = newSize;
		}

		glm::vec2 getCenter()
		{
			return this->center;
		}

		void changeCenter(glm::vec2 newCenter)
		{
			this->center = newCenter;
		}


	private:
		// Center is relative to the parent. (0, 0) is considered the middle of the parent
		glm::vec2 center;

		// If isRect, then this is (width, height). Else, this is (verticalRadius, horizontalRadius)
		glm::vec2 size;

		// Flag for whether it is an Axis aligned bounding box collider. This is used in the primary detection
		bool isAABB;

	};
}