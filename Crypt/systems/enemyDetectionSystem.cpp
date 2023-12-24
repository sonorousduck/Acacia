#include "enemyDetectionSystem.hpp"
#include <components/rigidbodyComponent.hpp>
#include <components/transform.hpp>
#include <iostream>

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
			std::cout << distance << std::endl;

			detectionComponent->canMoveTowardsTarget = distance < detectionComponent->movementRange;
			detectionComponent->canDetectTarget = distance < detectionComponent->detectionRange;

			if (transform->position != targetTransform->position)
			{
				detectionComponent->towardsTargetVector = glm::normalize(transform->position - targetTransform->position);
			}
			else
			{
				detectionComponent->towardsTargetVector = glm::vec2(0.0f, 0.0f);
			}

		}
	}
}

