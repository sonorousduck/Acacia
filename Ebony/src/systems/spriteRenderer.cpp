#include "spriteRenderer.hpp"

namespace systems
{
	void SpriteRenderer::Update(Ebony::Graphics2d& graphics)
	{
		for (auto&& [id, entity] : m_Entities)
		{
			auto sprite = entity->getComponent<components::Sprite>();
			auto transform = entity->getComponent<components::Transform>();


			graphics.Draw(sprite->shader, sprite->texture, transform->position, transform->scale * 50.0f, transform->rotation, sprite->spriteColor, sprite->depth);
		}
	}
}

