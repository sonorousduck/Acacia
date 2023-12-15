#pragma once

#include "component.hpp"
#include "../entity.hpp"

namespace components
{
	class ActiveUICollision : public PolymorphicComparable<Component, ActiveUICollision>
	{
	public:
		ActiveUICollision()
		{
		}

		entities::EntityPtr collidingWith;
	};
}