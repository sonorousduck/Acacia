#pragma once

#include "component.hpp"


namespace components
{
	class DestructionComponent : public PolymorphicComparable<Component, DestructionComponent>
	{
	public:
		DestructionComponent(std::function<void()> onDestroyCallback) : onDestroyCallback(onDestroyCallback)
		{
		}


		bool shouldDestroy = false;
		bool shouldRemove = false;
		std::function<void()> onDestroyCallback;

	};
}