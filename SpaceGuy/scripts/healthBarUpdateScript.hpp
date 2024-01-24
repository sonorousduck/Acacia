#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include "../components/enemyInformation.hpp"
#include <singletons/time.hpp>

namespace scripts
{
	struct HealthBarUpdateScript : components::CppScript
	{
	public:
		HealthBarUpdateScript(entities::EntityPtr parent) : parent(parent)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto parentTransform = parent->getComponent<components::Transform>();
			auto enemyInformation = parent->getComponent<components::EnemyInformation>();
			entity->getComponent<components::Transform>()->position = parentTransform->position + glm::vec2(0.0f, -20.0f);


			if (enemyInformation->health <= 0.0f)
			{
				entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
			}
			else
			{
				entity->getComponent<components::Transform>()->scale.x = (enemyInformation->health / enemyInformation->maxHealth) * parentTransform->scale.x;
			}

			
			
		}

		entities::EntityPtr parent;
	};
}