#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <singletons/systemManager.hpp>
#include <singletons/time.hpp>

#include <components/timedComponent.hpp>
#include "../prefabs/smallEnemyPrefab.hpp"

namespace scripts
{
	struct SpawnerScript : components::CppScript
	{
	public:
		SpawnerScript()
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			components::EnemyDetection* detectionComponent;
			if (entity->tryGetComponent<components::EnemyDetection>(detectionComponent))
			{
				if (detectionComponent->canDetectTarget)
				{
					auto spawningCooldown = entity->getComponent<components::TimedComponent>();

					if (spawningCooldown->currentTime <= 0.0f)
					{
						auto transform = entity->getComponent<components::Transform>();

						Ebony::SystemManager::AddEntity(SpaceGuy::SmallEnemy::Create(transform->position + glm::vec2(10.0f, 10.0f), glm::vec2(1.0f, 1.0f), detectionComponent->target, lastPointScore));
						lastPointScore -= 2.0f;

						lastPointScore = std::max(0.0f, lastPointScore);
						spawningCooldown->ResetTimer();
					}
				}				
			}
		}

		float lastPointScore = 10.0f;
	};
}