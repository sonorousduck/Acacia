#include "spriteRenderer.hpp"
#include "../misc/resourceManager.hpp"
#include "../components/collider.hpp"

namespace systems
{
	void SpriteRenderer::Update()
	{
		for (auto&& [id, entity] : m_Entities)
		{
			Ebony::Graphics2d::Draw(entity);


			/*if (debug && entity->hasComponent<components::Collider>())
			{
				auto collider = entity->getComponent<components::Collider>();
				Ebony::Graphics2d::Draw(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("collider"), transform->position + collider->aabbCollider.getCenter() - (collider->aabbCollider.getSize() / 2.0f), collider->aabbCollider.getSize(), transform->rotation, transform->rotationAxis, Ebony::Colors::Red, sprite->depth + 0.5f, sprite->isUI);
			}*/

		}
	}
}

