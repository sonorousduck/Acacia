#include "animationRenderer.hpp"

namespace systems
{
	void AnimationRenderer::Update(Animation2d& animationSystem, Ebony::Graphics2d& graphics)
	{

		for (auto&& [id, entity] : m_Entities)
		{
			auto animationController = entity->getComponent<components::AnimationController>();
			auto animationsToDraw = animationController->GetSprite();

			// Draw each Texture2D here
			for (auto& animation : animationsToDraw)
			{
				graphics.Draw(animation.GetSprite().spritesheet, glm::vec2(100.0f, 100.0f), glm::vec2(1.0f, 1.0f), 0.0f, Ebony::Colors::White);
			}


		}
		
		
		//graphics.Draw()


	}


}