#pragma once

#include "component.hpp"
#include "colors.hpp"
#include <spritefont.hpp>

namespace components
{
	class Text : public PolymorphicComparable<Component, Text>
	{
	public:
		Text(const std::string& text, glm::vec2 position, float scale, Ebony::Color color, Ebony::Color outlineColor, Ebony::SpriteFont spriteFont, float layerDepth = 0.0f, bool renderOutline = true, bool isHUDElement = false) :
			text(text),
			position(position),
			scale(scale),
			color(color),
			outlineColor(outlineColor),
			spriteFont(spriteFont),
			layerDepth(layerDepth),
			renderOutline(renderOutline),
			isHUDElement(isHUDElement)
		{}

		std::string text;
		Ebony::Color color;
		Ebony::Color outlineColor;
		Ebony::SpriteFont spriteFont;
		glm::vec2 position;
		float scale;
		bool renderOutline;
		float layerDepth;
		bool isHUDElement;
		

	};
}