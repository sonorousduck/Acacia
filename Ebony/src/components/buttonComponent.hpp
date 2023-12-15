#pragma once

#include "component.hpp"
#include "../entity.hpp"

namespace components
{
	class ButtonComponent : public PolymorphicComparable<Component, ButtonComponent>
	{
	public:
		ButtonComponent(const char* spriteImageUnpressed, const char* spriteImageHover, const char* spriteImagePressed) :
			spriteImageUnpressed(spriteImageUnpressed),
			spriteImageHover(spriteImageHover),
			spriteImagePressed(spriteImagePressed)
		{
		}

		const char* spriteImageUnpressed;
		const char* spriteImageHover;
		const char* spriteImagePressed;
		
		std::optional<std::function<void([[maybe_unused]] entities::EntityPtr)>> onPress;


	};
}