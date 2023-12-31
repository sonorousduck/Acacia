#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <singletons/time.hpp>
#include "../prefabs/bulletPrefab.hpp"

namespace scripts
{
	struct ShootingBatScript : components::CppScript
	{
	public:
		ShootingBatScript(std::function<void(entities::EntityPtr)> AddEntity) : AddEntity(AddEntity)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot


			components::EnemyDetection* detectionComponent;
			if (entity->tryGetComponent<components::EnemyDetection>(detectionComponent))
			{
				if (detectionComponent->canMoveTowardsTarget)
				{
					entity->getComponent<components::RigidBody>()->addScriptedMovement(-detectionComponent->towardsTargetVector * detectionComponent->movementSpeed * Ebony::Time::GetDeltaTimeFloat());
				}

				if (detectionComponent->canDetectTarget)
				{
					auto shootingCooldown = entity->getComponent<components::Shooting>();

					if (shootingCooldown->currentCooldown >= shootingCooldown->maxShootingSpeed)
					{
						AddEntity(Crypt::BulletPrefab::Create(entity->getComponent<components::Transform>()->position, glm::vec2(35.0f, 35.0f), -detectionComponent->towardsRealTargetVector, detectionComponent->shootSpeed, components::BULLET_TYPE::ENEMY_BAT, 1, detectionComponent->shootSprite, 3.0f, Crypt::CollisionLayers::ENEMY_BULLET, Crypt::CollisionLayers::PLAYER | Crypt::CollisionLayers::GROUND, 90.0f));
						shootingCooldown->currentCooldown -= shootingCooldown->maxShootingSpeed;
					}


					// Shoot!
				}
			}
		}


		std::function<void(entities::EntityPtr)> AddEntity;
	};
}