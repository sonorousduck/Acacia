#pragma once

#include <iostream>
#include "../Ebony/src/components/cppScriptComponent.hpp"
#include "../BrickBreaker/screens/screenEnums.hpp"
#include <components/transform.hpp>

namespace scripts
{
	struct CrosshairScript : components::CppScript
	{
	public:
		CrosshairScript(entities::EntityPtr player) : player(player)
		{
		}

		void MoveCrosshair(int screenPositionX, int screenPositionY)
		{
			// Keep a radius of 1 away from the player, but point towards the direction
			auto playerTransform = player->getComponent<components::Transform>();

			glm::vec2 currentDirection = glm::normalize(glm::vec2(screenPositionX, screenPositionY) + (playerTransform->position - playerTransform->previousPosition)) * glm::vec2(100, -100);

			aimLastDirection = currentDirection;
			std::cout << (aimLastDirection + playerTransform->position).x << ", " << (aimLastDirection + playerTransform->position).y << std::endl;
			entity->getComponent<components::Transform>()->position = aimLastDirection + playerTransform->position;
		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			auto mouseComponent = entity->getComponent<components::MouseInput>();
			MoveCrosshair(mouseComponent->positionX, mouseComponent->positionY);
		}

		entities::EntityPtr player;
		glm::vec2 aimLastDirection{};


	};
}