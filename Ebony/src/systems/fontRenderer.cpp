#include "fontRenderer.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void FontRenderer::Update(std::shared_ptr<Ebony::Graphics2d> graphics)
	{
		glDisable(GL_DEPTH_TEST);

		for (auto&& [id, entity] : m_Entities)
		{
			auto text = entity->getComponent<components::Text>();
			auto transform = entity->getComponent<components::Transform>();



			// If outline border, we are going to draw 1 up, left, right and down.
			if (text->renderOutline)
			{
				graphics->DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position.x + 1, transform->position.y, transform->scale.x, text->outlineColor, text->color, text->layerDepth); // Right
				graphics->DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position.x - 1, transform->position.y, transform->scale.x, text->outlineColor, text->color, text->layerDepth); // Left
				graphics->DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position.x, transform->position.y - 1, transform->scale.x, text->outlineColor, text->color, text->layerDepth); // Up
				graphics->DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position.x, transform->position.y + 1, transform->scale.x, text->outlineColor, text->color, text->layerDepth); // Down
			}

			graphics->DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position.x, transform->position.y, transform->scale.x, text->color, text->outlineColor, text->layerDepth);

		}
		glEnable(GL_DEPTH_TEST);

	}


}