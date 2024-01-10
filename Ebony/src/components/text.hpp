#pragma once

#include "component.hpp"
#include "colors.hpp"
#include "spritefont.hpp"
#include "../misc/renderLayers.hpp"
#include "../misc/resourceManager.hpp"
#include "texture.hpp"

namespace components
{
	class Text : public PolymorphicComparable<Component, Text>
	{
	public:
		Text(const std::string& text, Ebony::Color color, Ebony::Color outlineColor, std::shared_ptr<Ebony::SpriteFont> spriteFont, Ebony::RenderLayer renderLayer, float layerDepth = 0.0f, bool renderOutline = true, bool isUI = false, glm::vec2 scale = {1.0f, 1.0f}) :
			text(text),
			color(color),
			outlineColor(outlineColor),
			spriteFont(spriteFont),
			renderLayer(renderLayer),
			layerDepth(layerDepth),
			renderOutline(renderOutline),
			isUI(isUI),
			scale(scale)
		{}

		std::string text;
		Ebony::Color color;
		Ebony::Color outlineColor;
		glm::vec2 scale;
		std::shared_ptr<Ebony::SpriteFont> spriteFont;
		bool renderOutline;
		float layerDepth;
		std::uint16_t renderLayer;
		std::shared_ptr<Shader> shader = Ebony::ResourceManager::GetShader("text");
		bool isUI;
		

	};
}