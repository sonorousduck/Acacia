#include "fontRenderer.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void FontRenderer::Update()
	{
		glDisable(GL_DEPTH_TEST);

		for (auto&& [id, entity] : m_Entities)
		{
			auto text = entity->getComponent<components::Text>();
			auto transform = entity->getComponent<components::Transform>();



			// If outline border, we are going to draw 1 up, left, right and down.
			if (text->renderOutline)
			{
				Ebony::Graphics2d::DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position + glm::vec2(1.0f, 0.0f), transform->scale, transform->rotation, text->outlineColor, text->color, text->layerDepth); // Right
				Ebony::Graphics2d::DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position + glm::vec2(-1.0f, 0.0f), transform->scale, transform->rotation, text->outlineColor, text->color, text->layerDepth); // Left
				Ebony::Graphics2d::DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position + glm::vec2(0.0f, -1.0f), transform->scale, transform->rotation, text->outlineColor, text->color, text->layerDepth); // Up
				Ebony::Graphics2d::DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position + glm::vec2(0.0f, 1.0f), transform->scale, transform->rotation, text->outlineColor, text->color, text->layerDepth); // Down
			}

			Ebony::Graphics2d::DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position, transform->scale, transform->rotation, text->color, text->outlineColor, text->layerDepth);

		}
		glEnable(GL_DEPTH_TEST);

	}


}