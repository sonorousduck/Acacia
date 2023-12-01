#include "animationRenderer.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void AnimationRenderer::Update(std::shared_ptr<Ebony::Graphics2d> graphics)
	{

		for (auto&& [id, entity] : m_Entities)
		{
			auto animationController = entity->getComponent<components::AnimationController>();
			auto transform = entity->getComponent<components::Transform>();
			auto animationsToDraw = animationController->GetSprite();

			// Draw each Texture2D here
			for (auto& animation : animationsToDraw)
			{
				graphics->DrawAnimation(Ebony::ResourceManager::GetShader("spritesheet"), animation.GetSprite().spritesheet, animation.GetCurrentSpriteFrame(), transform->position, transform->scale, transform->rotation, Ebony::Colors::Red, animation.GetDepth());
			}


		}
		
		
		//graphics.Draw()


	}


}