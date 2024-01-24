#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <singletons/systemManager.hpp>
#include <singletons/time.hpp>
#include "../prefabs/enemyBullet.hpp"
#include <components/timedComponent.hpp>
#include "../prefabs/enemyBullet.hpp"

namespace scripts
{
	struct EnemyMovementShootingScript : components::CppScript
	{
	public:
		EnemyMovementShootingScript()
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			components::EnemyDetection* detectionComponent;
			if (entity->tryGetComponent<components::EnemyDetection>(detectionComponent))
			{
				auto test = detectionComponent->tolerance;

				if (detectionComponent->canMoveTowardsTarget)
				{
					if (detectionComponent->tolerance > detectionComponent->distanceAway)
					{
						// Slow down movement
						auto test = glm::vec2(detectionComponent->tolerance / detectionComponent->distanceAway);
						entity->getComponent<components::RigidBody>()->addScriptedMovement(-detectionComponent->towardsTargetVector * detectionComponent->movementSpeed * Ebony::Time::GetDeltaTimeFloat() * glm::vec2(detectionComponent->distanceAway / detectionComponent->tolerance));
						
					
						entity->getComponent<components::Transform>()->rotation = glm::degrees(glm::atan(-detectionComponent->towardsRealTargetVector.x, detectionComponent->towardsRealTargetVector.y));
					
					}
					else
					{
						entity->getComponent<components::RigidBody>()->addScriptedMovement(-detectionComponent->towardsTargetVector * detectionComponent->movementSpeed * Ebony::Time::GetDeltaTimeFloat());
						entity->getComponent<components::Transform>()->rotation = glm::degrees(glm::atan(-detectionComponent->towardsRealTargetVector.x, detectionComponent->towardsRealTargetVector.y));

					}


				}

				if (detectionComponent->canDetectTarget)
				{
					auto shootingCooldown = entity->getComponent<components::TimedComponent>();

					if (shootingCooldown->currentTime <= 0.0f)
					{
						auto transform = entity->getComponent<components::Transform>();

						Ebony::SystemManager::AddEntity(SpaceGuy::EnemyBullet::Create(transform->position, transform->rotation, 1.0f, "enemy_bullet"));
						shootingCooldown->ResetTimer();
					}
				}
			}
		}
	};
}