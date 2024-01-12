#pragma once
#include <entity.hpp>
#include <SDL.h>
#include <components/controllerComponent.hpp>
#include <components/keyboardComponent.hpp>
#include "../screens/screenEnums.hpp"
#include <singletons/systemManager.hpp>

namespace Crypt
{
	class GameOverControlsPrefab
	{
	public:
		static entities::EntityPtr Create()
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			std::unique_ptr<components::ControllerInput> controllerInputComponent = std::make_unique<components::ControllerInput>(0);
			std::unique_ptr<components::KeyboardInput> keyboardInputComponent = std::make_unique<components::KeyboardInput>();


			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_START, "quit" });
			keyboardInputComponent->bindings.insert({ SDLK_ESCAPE, "quit" });

			keyboardInputComponent->onPressActions.insert({ "quit", [=]()
				{
					Ebony::SystemManager::nextScreenEnum = ScreenEnum::QUIT;
				} });

			controllerInputComponent->onPressActions.insert({ "quit", [=]()
				{
					Ebony::SystemManager::nextScreenEnum = ScreenEnum::QUIT;
				} });


			controllerInputComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_A, "newGame" });
			keyboardInputComponent->bindings.insert({ SDLK_RETURN, "newGame" });

			keyboardInputComponent->onPressActions.insert({ "newGame", [=]()
				{
					Ebony::SystemManager::nextScreenEnum = ScreenEnum::GAME;
				} });

			controllerInputComponent->onPressActions.insert({ "newGame", [=]()
				{
					Ebony::SystemManager::nextScreenEnum = ScreenEnum::GAME;
				} });
			

			
			entity->addComponent(std::move(keyboardInputComponent));
			entity->addComponent(std::move(controllerInputComponent));

			return entity;
		}

	};
}