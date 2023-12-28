#include "tileRenderingSystem.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void TileRenderer::Update()
	{
		for (auto&& [id, entity] : m_Entities)
		{
			auto sprite = entity->getComponent<components::Sprite>();
			auto transform = entity->getComponent<components::Transform>();


			Ebony::Graphics2d::Draw(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, transform->rotationAxis, sprite->spriteColor, sprite->depth, sprite->isUI);
		}
	}
}

