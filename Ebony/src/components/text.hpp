#pragma once

#include "component.hpp"
#include "colors.hpp"
#include <spritefont.hpp>

namespace components
{
	class Text : public PolymorphicComparable<Component, Text>
	{
	public:
		Text(const std::string& text, Ebony::Color color, Ebony::Color outlineColor, std::shared_ptr<Ebony::SpriteFont> spriteFont, float layerDepth = 0.0f, bool renderOutline = true, bool isUI = false) :
			text(text),
			color(color),
			outlineColor(outlineColor),
			spriteFont(spriteFont),
			layerDepth(layerDepth),
			renderOutline(renderOutline),
			isUI(isUI)
		{}

		std::string text;
		Ebony::Color color;
		Ebony::Color outlineColor;
		std::shared_ptr<Ebony::SpriteFont> spriteFont;
		bool renderOutline;
		float layerDepth;
		bool isUI;
		

	};
}