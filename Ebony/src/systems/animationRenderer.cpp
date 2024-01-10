#include "animationRenderer.hpp"
#include "../misc/resourceManager.hpp"

namespace systems
{
	void AnimationRenderer::Update()
	{

		for (auto&& [id, entity] : m_Entities)
		{
			auto animationController = entity->getComponent<components::AnimationController>();
			auto transform = entity->getComponent<components::Transform>();
			auto animationsToDraw = animationController->GetSprite();
			

			Ebony::Graphics2d::DrawAnimation(entity);

			//// Draw each Texture2D here
			//for (auto& animation : animationsToDraw)
			//{
			//	//Ebony::Graphics2d::DrawAnimation(Ebony::ResourceManager::GetShader("spritesheet"), animation.GetSprite().spritesheet, animation.GetCurrentSpriteFrame(), transform->position, transform->scale, transform->rotation, transform->rotationAxis, Ebony::Colors::Red, animation.GetDepth(), animationController->isUI);
			//	Ebony::Graphics2d::DrawAnimation(entity, animation);

			//}


		}
		
		
		//graphics.Draw()


	}


}