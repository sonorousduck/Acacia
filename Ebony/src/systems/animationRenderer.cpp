#include "animationRenderer.hpp"
#include <resourceManager.hpp>

namespace systems
{
	void AnimationRenderer::Update(Ebony::Graphics2d& graphics)
	{

		for (auto&& [id, entity] : m_Entities)
		{
			auto animationController = entity->getComponent<components::AnimationController>();
			auto animationsToDraw = animationController->GetSprite();

			// Draw each Texture2D here
			for (auto& animation : animationsToDraw)
			{
				graphics.DrawAnimation(Ebony::ResourceManager::GetShader("spritesheet"), animation.GetSprite().spritesheet, animation.GetCurrentSpriteFrame(), glm::vec2(200.0f, 100.0f), glm::vec2(100.0f, 100.0f), 0.0f, Ebony::Colors::Red, animation.GetDepth());
			}


		}
		
		
		//graphics.Draw()


	}


}