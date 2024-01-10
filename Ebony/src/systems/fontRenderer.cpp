#include "fontRenderer.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void FontRenderer::Update()
	{
		//glDisable(GL_DEPTH_TEST);

		for (auto&& [id, entity] : m_Entities)
		{
			auto text = entity->getComponent<components::Text>();
			auto transform = entity->getComponent<components::Transform>();



			// If outline border, we are going to draw 1 up, left, right and down.
			if (text->renderOutline)
			{
				Ebony::Graphics2d::DrawString(entity, glm::vec2(1.0f, 0.0f)); // Right
				Ebony::Graphics2d::DrawString(entity, glm::vec2(-1.0f, 0.0f)); // Left
				Ebony::Graphics2d::DrawString(entity, glm::vec2(0.0f, -1.0f)); // Up
				Ebony::Graphics2d::DrawString(entity, glm::vec2(0.0f, 1.0f)); // Down
			}

			Ebony::Graphics2d::DrawString(entity, glm::vec2(0.0f, 0.0f));

			//Ebony::Graphics2d::DrawString(Ebony::ResourceManager::GetShader("text"), text->spriteFont, text->text, transform->position, text->scale, transform->scale, transform->rotation, transform->rotationAxis, text->color, text->outlineColor, text->layerDepth + 0.01f, text->isUI);

		}
		//glEnable(GL_DEPTH_TEST);

	}
}