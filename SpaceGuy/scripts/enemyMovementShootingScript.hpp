#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <singletons/systemManager.hpp>
#include <singletons/time.hpp>
#include "../prefabs/enemyBullet.hpp"
#include <components/timedComponent.hpp>

namespace scripts
{
	struct EnemyMovementShootingScript : components::CppScript
	{
	public:
		EnemyMovementShootingScript(float bulletDamage = 1.0f, const char* soundEffect = "enemy_shoot", glm::vec2 scale = glm::vec2(1.0f, 1.0f)) : bulletDamage(bulletDamage), soundEffect(soundEffect), scale(scale)
		{
		}

		float bulletDamage;
		const char* soundEffect;
		glm::vec2 scale;

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


				else if (detectionComponent->movementRange == 0.0f)
				{
					entity->getComponent<components::Transform>()->rotation = glm::degrees(glm::atan(-detectionComponent->towardsRealTargetVector.x, detectionComponent->towardsRealTargetVector.y));
				}

				if (detectionComponent->canDetectTarget)
				{
					auto shootingCooldown = entity->getComponent<components::TimedComponent>();

					if (shootingCooldown->currentTime <= 0.0f)
					{
						auto transform = entity->getComponent<components::Transform>();

						Ebony::SystemManager::AddEntity(SpaceGuy::EnemyBullet::Create(transform->position + glm::vec2(transform->scale.x / 2.0f, 0.0f), transform->rotation, bulletDamage, "enemy_bullet", soundEffect, scale));
						shootingCooldown->ResetTimer();
					}
				}
			}
		}
	};
}