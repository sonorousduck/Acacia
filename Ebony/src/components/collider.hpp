#pragma once

#include <optional>
#include <functional>

#include "component.hpp"
#include "../entity.hpp"
#include <box2d/b2_shape.h>

namespace components
{
	class Collider : public PolymorphicComparable<Component, Collider>
	{
	public:
		Collider() {};

		std::unique_ptr<b2Shape> collider;

		std::optional<std::function<void(entities::EntityPtr, entities::EntityPtr)>> onCollisionStart;
		std::optional<std::function<void(entities::EntityPtr, entities::EntityPtr)>> onCollision;
		std::optional<std::function<void(entities::EntityPtr, entities::EntityPtr)>> onCollisionEnd;



	};
}

