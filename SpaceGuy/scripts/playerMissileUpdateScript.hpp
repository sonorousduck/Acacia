#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>
#include "../components/playerInformation.hpp"
#include "../components/playerShootingInformation.hpp"
#include <components/text.hpp>

namespace scripts
{
	struct PlayerMissileScript : components::CppScript
	{
	public:
		PlayerMissileScript(entities::EntityPtr parent) : parent(parent)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto player = parent->getComponent<components::PlayerShootingInformation>();
			auto text = entity->getComponent<components::Text>();


			text->text = "x " + std::to_string(static_cast<int>(player->missileCount));
		}

		entities::EntityPtr parent;

	};
}