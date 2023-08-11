#include "fontRenderer.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void FontRenderer::Update(Ebony::Graphics2d& graphics)
	{
		glDisable(GL_DEPTH_TEST);

		for (auto&& [id, entity] : m_Entities)
		{
			auto text = entity->getComponent<components::Text>();



			// If outline border, we are going to draw 1 up, left, right and down.
			if (text->renderOutline)
			{
				graphics.DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, text->position.x + 1, text->position.y, text->scale, text->outlineColor, text->color, text->layerDepth); // Right
				graphics.DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, text->position.x - 1, text->position.y, text->scale, text->outlineColor, text->color, text->layerDepth); // Left
				graphics.DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, text->position.x, text->position.y - 1, text->scale, text->outlineColor, text->color, text->layerDepth); // Up
				graphics.DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, text->position.x, text->position.y + 1, text->scale, text->outlineColor, text->color, text->layerDepth); // Down
			}

			graphics.DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, text->position.x, text->position.y, text->scale, text->color, text->outlineColor, text->layerDepth);

		}
		glEnable(GL_DEPTH_TEST);

	}


}