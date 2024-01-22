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
			if (!entity->isEnabled()) return;


			auto sprite = entity->getComponent<components::AnimationController>();

			// This updates the current node's animations. Also will call any callbacks registered to the sprite frames for the animations
			for (auto& animation : sprite->animationTree.at(sprite->currentNode).animations)
			{
				animation.updateElapsedTime(elapsedTime);
				
				if (auto keyframeAnimation = animation.onAnimationFrame.find(animation.GetCurrentSpriteFrame()); keyframeAnimation != animation.onAnimationFrame.end())
				{
					for (const auto& callback : keyframeAnimation->second)
					{
						callback();
					}
				}
			}

			// This defines how it should move to different animation nodes
			for (const auto& link : sprite->animationTree.at(sprite->currentNode).links)
			{
				if (link.shouldTraverse())
				{
					for (auto& animation : sprite->animationTree.at(sprite->currentNode).animations)
					{
						animation.ResetAnimation();
					}
					sprite->currentNode = link.childNode;
					break;
				}
			}
		}
	}


}