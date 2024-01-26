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
	struct BossShootingScript : components::CppScript
	{
	public:
		BossShootingScript(float bulletDamage = 1.0f, const char* soundEffect = "enemy_shoot", glm::vec2 scale = glm::vec2(1.0f, 1.0f)) : bulletDamage(bulletDamage), soundEffect(soundEffect), scale(scale)
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
				if (detectionComponent->canDetectTarget)
				{
					auto shootingCooldown = entity->getComponent<components::TimedComponent>();

					if (shootingCooldown->currentTime <= 0.0f)
					{
						auto transform = entity->getComponent<components::Transform>();

						// Shoot in a 360
						for (int i = 0; i < 36; i++)
						{
							Ebony::SystemManager::AddEntity(SpaceGuy::EnemyBullet::Create(transform->position + transform->scale / 2.0f, static_cast<float>(i * 10), bulletDamage, "enemy_bullet", soundEffect, scale, i == 0));
						}

						shootingCooldown->ResetTimer();
					}
				}
			}
		}
	};
}