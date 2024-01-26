#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>
#include "../components/playerInformation.hpp"
#include <components/text.hpp>

namespace scripts
{
	struct PlayerOverallLivesScript : components::CppScript
	{
	public:
		PlayerOverallLivesScript(entities::EntityPtr parent) : parent(parent)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto player = parent->getComponent<components::PlayerInformation>();
			auto text = entity->getComponent<components::Text>();


			text->text = "x " + std::to_string(player->lives);
		}

		entities::EntityPtr parent;

	};
}