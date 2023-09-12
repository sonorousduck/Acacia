#pragma once

#include <optional>
#include <functional>

#include "component.hpp"
#include "../entity.hpp"

namespace components
{
	class Collider : public PolymorphicComparable<Component, Collider>
	{
	public:
		Collider() {};



		std::optional<std::function<void(entities::EntityPtr, entities::EntityPtr)>> onCollisionStart;
		std::optional<std::function<void(entities::EntityPtr, entities::EntityPtr)>> onCollision;
		std::optional<std::function<void(entities::EntityPtr, entities::EntityPtr)>> onCollisionEnd;



	};
}

