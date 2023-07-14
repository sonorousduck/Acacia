#include "animation2d.hpp"

namespace systems
{
	void Animation2d::Update(const std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void Animation2d::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto&& [id, entity] : m_Entities)
		{
			auto sprite = entity->getComponent<components::AnimationController>();

			for (auto& animation : sprite->animations.at(sprite->currentNode))
			{
				animation.updateElapsedTime(elapsedTime);
			}

			for (const auto& link : sprite->animationTree.at(sprite->currentNode).links)
			{
				if (link.shouldTraverse())
				{
					for (auto& animation : sprite->animations.at(sprite->currentNode))
					{
						animation.ResetAnimation();
					}
					sprite->currentNode = link.childNode;
					break;
				}
			}



				//for (const auto& function : animation.onAnimationUpdate)
				//{
				//	function();
				//}

				//if (animation.getElapsedTime() > animation.GetSpriteTime())
				//{
				//	// If we are on the last sprite, we need to make 2 checks
				//	// 1. If the numRepeat > 0, decrement and then increment sprite
				//	// 2. If the bool repeatForever, increment sprite
				//	// 3. 

				//	if (animation.GetCurrentSprite() == animation.GetSprite().numDivisions)
				//	{
				//		if (animation.)


				//	}


				//	animation.IncrementSprite();

					// Do a check if it is on the end

		}
	}


}