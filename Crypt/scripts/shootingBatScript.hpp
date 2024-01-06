#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <singletons/systemManager.hpp>
#include <singletons/time.hpp>
#include "../prefabs/bulletPrefab.hpp"

namespace scripts
{
	struct ShootingBatScript : components::CppScript
	{
	public:
		ShootingBatScript()
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
					}
					else
					{
						entity->getComponent<components::RigidBody>()->addScriptedMovement(-detectionComponent->towardsTargetVector * detectionComponent->movementSpeed * Ebony::Time::GetDeltaTimeFloat());
					}


				}

				if (detectionComponent->canDetectTarget)
				{
					auto shootingCooldown = entity->getComponent<components::Shooting>();

					if (shootingCooldown->currentCooldown >= shootingCooldown->maxShootingSpeed)
					{
						Ebony::SystemManager::AddEntity(Crypt::BulletPrefab::Create(entity->getComponent<components::Transform>()->position, glm::vec2(1.0f, 1.0f), -detectionComponent->towardsRealTargetVector, detectionComponent->shootSpeed, components::BULLET_TYPE::ENEMY_BAT, 1, detectionComponent->shootSprite, 3.0f, Crypt::CollisionLayers::ENEMY_BULLET, Crypt::CollisionLayers::PLAYER | Crypt::CollisionLayers::GROUND, 90.0f));
						shootingCooldown->currentCooldown -= shootingCooldown->maxShootingSpeed;
					}
				}
			}
		}
	};
}