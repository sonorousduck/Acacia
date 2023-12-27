#include "enemyDetectionSystem.hpp"
#include <components/rigidbodyComponent.hpp>
#include <components/transform.hpp>
#include <iostream>
#include "../components/playerComponent.hpp"

namespace systems
{
	void EnemyDetectionSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void EnemyDetectionSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto detectionComponent = entity->getComponent<components::EnemyDetection>();


			auto transform = entity->getComponent<components::Transform>();
			auto targetTransform = detectionComponent->target->getComponent<components::Transform>();


			float distance = glm::distance(transform->position, targetTransform->position);
			
			detectionComponent->canMoveTowardsTarget = distance < detectionComponent->movementRange;
			detectionComponent->canDetectTarget = distance < detectionComponent->detectionRange;
			if (transform->position != targetTransform->position + detectionComponent->offset)
			{
				components::Player* playerComponent;
				if (detectionComponent->target->tryGetComponent(playerComponent))
				{
					// If it is targetting the player, you want to check if it has gravity
					glm::vec2 towardsTargetVector = glm::normalize(transform->position - (targetTransform->position + detectionComponent->offset * glm::vec2(1.0f, playerComponent->gravityDown ? -1 : 1)));
					glm::vec2 towardsRealTarget = glm::normalize(transform->position - targetTransform->position);

					if (glm::isnan(towardsTargetVector.x) || glm::isnan(towardsTargetVector.y))
					{
						detectionComponent->towardsTargetVector = glm::vec2(0.0f, 0.0f);
					}
					else
					{
						detectionComponent->towardsTargetVector = towardsTargetVector;
					}

					detectionComponent->towardsRealTargetVector = towardsRealTarget;
				}
				else
				{
					detectionComponent->towardsTargetVector = glm::normalize(transform->position - (targetTransform->position + detectionComponent->offset));
				}
			}
		}
	}
}

