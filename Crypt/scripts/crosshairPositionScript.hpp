#pragma once
#include <components/controllerComponent.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/aiInputComponent.hpp>

#include <iostream>
#include "../Ebony/src/components/cppScriptComponent.hpp"
#include "../BrickBreaker/screens/screenEnums.hpp"
#include <components/transform.hpp>
#include <graphics2d.hpp>
#include "../components/aimComponent.hpp"

namespace scripts
{
	struct CrosshairScript : components::CppScript
	{
	public:
		CrosshairScript(entities::EntityPtr player) : player(player)
		{
		}

		void MoveCrosshair(float screenPositionX, float screenPositionY)
		{
			// Keep a radius of 1 away from the player, but point towards the direction
			auto playerTransform = player->getComponent<components::Transform>();
			glm::vec3 cameraPosition = Ebony::Graphics2d::mainCamera->Position;

			glm::vec2 fireFromPosition = player->getComponent<components::Transform>()->position;

			//if (!player->getComponent<components::Player>()->gravityDown)
			//{
			//	fireFromPosition += glm::vec2(0.0f, 10.0f);
			//}
			//else
			//{
			//	fireFromPosition -= glm::vec2(0.0f, 10.0f);
			//}

			glm::vec2 currentDirection = glm::normalize((glm::vec2(screenPositionX, screenPositionY) + glm::vec2(cameraPosition.x, cameraPosition.y)) - fireFromPosition) * glm::vec2(100, 100);

			aimLastDirection = currentDirection;
			//entity->getComponent<components::Transform>()->position = aimLastDirection + playerTransform->position;
		}

		void MoveCrosshairController(float x, float y)
		{
			if (x != 0.0f && y != 0.0f)
			{
				glm::vec3 cameraPosition = Ebony::Graphics2d::mainCamera->Position;
				glm::vec2 currentDirection = glm::normalize(glm::vec2(x, y)) * glm::vec2(100, 100);

				aimLastDirection = currentDirection;
			}
		}

		void MoveCrosshairAI()
		{
			auto& action = Crypt::CryptPythonManager::action;

			auto& angle = action.box[1];
			glm::vec2 fireFromPosition = entity->getComponent<components::Transform>()->position;

			glm::vec3 cameraPosition = Ebony::Graphics2d::mainCamera->Position;
			glm::vec2 currentDirection = glm::vec2(glm::cos(angle), glm::sin(angle)) * glm::vec2(100, 100);

			aimLastDirection = currentDirection;
		}

		void Update(std::chrono::microseconds elapsedTime) override
		{
			components::MouseInput* mouseInput;
			components::ControllerInput* controllerInput;
			components::AiInput* aiComponent;

			if (entity->tryGetComponent(mouseInput))
			{
				// Need to check so if the player is using controller it doesn't keep setting back
				if (mouseInput->positionX != mouseInput->previousX || mouseInput->positionY != mouseInput->previousY)
				{
					MoveCrosshair(mouseInput->positionX, mouseInput->positionY);
				}

			}

			if (entity->tryGetComponent(controllerInput))
			{
				if (controllerInput->rightJoystickXPosition != controllerInput->rightLastJoystickXPosition
					|| controllerInput->rightJoystickYPosition != controllerInput->rightLastJoystickYPosition
					&& (controllerInput->rightJoystickXPosition != 0.0f && controllerInput->rightJoystickYPosition != 0.0f))
				{
					MoveCrosshairController(controllerInput->rightJoystickXPosition, controllerInput->rightJoystickYPosition);
				}
			}


			if (Ebony::SystemManager::aiEnabled && entity->tryGetComponent(aiComponent))
			{
				MoveCrosshairAI();
			}

			auto playerTransform = player->getComponent<components::Transform>();
			entity->getComponent<components::Transform>()->position = aimLastDirection + playerTransform->position;

			auto crosshairComponent = entity->getComponent<components::Crosshair>();
			crosshairComponent->aimDirection = aimLastDirection;
			crosshairComponent->aimLocation = aimLastDirection + playerTransform->position;

		}

		entities::EntityPtr player;
		glm::vec2 aimLastDirection{};

	};
}