#include "spriteRenderer.hpp"
#include "../misc/resourceManager.hpp"
#include "../components/collider.hpp"

namespace systems
{
	void SpriteRenderer::Update(Ebony::Graphics2d& graphics)
	{
		for (auto&& [id, entity] : m_Entities)
		{
			auto sprite = entity->getComponent<components::Sprite>();
			auto transform = entity->getComponent<components::Transform>();


			graphics.Draw(sprite->shader, sprite->texture, transform->position, transform->scale, transform->rotation, sprite->spriteColor, sprite->depth);


			if (debug && entity->hasComponent<components::Collider>())
			{
				graphics.Draw(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("collider"), transform->position, transform->scale, transform->rotation, Ebony::Colors::Red, sprite->depth + 0.1f);
			}

		}
	}
}

