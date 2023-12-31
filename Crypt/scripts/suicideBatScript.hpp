#pragma once

#include <components/rigidbodyComponent.hpp>
#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include <singletons/time.hpp>

namespace scripts
{
	struct SuicideBatScript : components::CppScript
	{
	public:
		SuicideBatScript()
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
			}
		}


	};
}