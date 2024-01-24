#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>
#include "../components/playerInformation.hpp"
#include <components/text.hpp>

namespace scripts
{
	struct PlayerScoreUpdateScript : components::CppScript
	{
	public:
		PlayerScoreUpdateScript(entities::EntityPtr parent) : parent(parent)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto player = parent->getComponent<components::PlayerInformation>();
			auto text = entity->getComponent<components::Text>();


			text->text = std::to_string(player->score);

		}

		entities::EntityPtr parent;

	};
}