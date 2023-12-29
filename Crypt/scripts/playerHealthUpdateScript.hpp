#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>
#include "../components/playerComponent.hpp"
#include <components/text.hpp>

namespace scripts
{
	struct PlayerHealthUpdateScript : components::CppScript
	{
	public:
		PlayerHealthUpdateScript(entities::EntityPtr parent) : parent(parent)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto player = parent->getComponent<components::Player>();

			auto textComponent = entity->getComponent<components::Text>();

			textComponent->text = std::to_string(player->health) + "/10";
			



		}

		entities::EntityPtr parent;
	};
}