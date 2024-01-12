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
		Text(const std::string& text, Ebony::Color color, Ebony::Color outlineColor, std::shared_ptr<Ebony::SpriteFont> spriteFont, Ebony::RenderLayer renderLayer, float layerDepth = 0.0f, bool renderOutline = true, bool isUI = true, glm::vec2 scale = {1.0f, 1.0f}) :
			text(text),
			color(color),
			outlineColor(outlineColor),
			spriteFont(spriteFont),
			renderLayer(renderLayer),
			layerDepth(layerDepth),
			renderOutline(renderOutline),
			isUI(isUI),
			scale(scale)
		{
			stringSize = MeasureString();
		}

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
		glm::vec2 stringSize;
		

		// Measure it the same way we render it.
		glm::vec2 MeasureString()
		{
			glm::vec2 tempSize{};

			std::string::const_iterator c;
			for (c = text.begin(); c != text.end(); c++)
			{
				Ebony::Character ch = spriteFont->characters[*c];

				float xpos = ch.Bearing.x * scale.x;
				float ypos = (ch.Size.y - ch.Bearing.y) * scale.y;

				float w = ch.Size.x * scale.x;
				float h = ch.Size.y * scale.y;	

				tempSize.x += xpos + w;
				tempSize.y += ypos + h;
			}
		
			return tempSize;
		};

	};
}