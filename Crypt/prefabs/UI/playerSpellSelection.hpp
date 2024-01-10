#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <misc/resourceManager.hpp>
#include <components/sprite.hpp>

#include "../UI/spellPrefab.hpp"
#include "../../components/selectedSpellComponent.hpp"
#include <misc/renderLayers.hpp>


namespace Crypt
{
	class PlayerSpellSelection
	{
	public:
		static entities::EntityPtr Create(int windowHeight, entities::EntityPtr crosshair)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("panel"), Ebony::Colors::White, Ebony::RenderLayer::UI_RENDER, true);
			//std::unique_ptr<components::CppScript> script = std::make_unique<scripts::PlayerHealthUpdateScript>(player);
			glm::vec2 overallScaling = glm::vec2(2.0f, 2.0f);

			glm::vec2 initialUIPlacement = glm::vec2(0.0f, static_cast<float>(windowHeight) - (sprite->texture->Height * overallScaling.y));
			
			auto selectedSpellComponent = std::make_unique<components::SelectedSpell>();



			//auto textComponent = std::make_unique<components::Text>("10/10", Ebony::Colors::Black, Ebony::Colors::White, Ebony::ResourceManager::GetFont("evil_empire"), 0.15f, true, true, glm::vec2(1.0f, 1.0f));
			
			auto fireSpellEntity = SpellPrefab::Create(entity, initialUIPlacement + glm::vec2(15.0f, 10.0f), glm::vec2(2.0f, 2.0f), "fire_bullet_panel", true);
			auto iceSpellEntity = SpellPrefab::Create(entity, initialUIPlacement + glm::vec2(15.0f, 60.0f), glm::vec2(2.0f, 2.0f), "ice_bullet_panel");


			auto keyboardComponent = std::make_unique<components::KeyboardInput>();
			auto controllerComponent = std::make_unique<components::ControllerInput>(0);


			keyboardComponent->bindings.insert({ SDLK_1, "firstSpell" });
			keyboardComponent->bindings.insert({ SDLK_2, "secondSpell" });
			controllerComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_DPAD_UP, "nextSpell" });
			controllerComponent->bindings.insert({ SDL_CONTROLLER_BUTTON_DPAD_DOWN, "nextSpell" });



			keyboardComponent->onPressActions.insert({"secondSpell", [=]()
			{
				iceSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Color(glm::vec4(0.5647f, 0.933f, 0.5647f, 0.6f));
				fireSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Colors::White;

				entity->getComponent<components::SelectedSpell>()->selectedSpell = 1;
				crosshair->getComponent<components::Crosshair>()->bulletType = components::BULLET_TYPE::ICE;
			}
			});

			keyboardComponent->onPressActions.insert({"firstSpell", [=]()
			{
				iceSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Colors::White;
				fireSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Color(glm::vec4(0.5647f, 0.933f, 0.5647f, 0.6f));
				crosshair->getComponent<components::Crosshair>()->bulletType = components::BULLET_TYPE::FIRE;


				entity->getComponent<components::SelectedSpell>()->selectedSpell = 0;
			}
			});

			controllerComponent->onPressActions.insert({ "nextSpell", [=]()
				{
					entity->getComponent<components::SelectedSpell>()->selectedSpell = (entity->getComponent<components::SelectedSpell>()->selectedSpell + 1) % 2;
					
					switch (entity->getComponent<components::SelectedSpell>()->selectedSpell)
					{
					case 0:
						iceSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Colors::White;
						fireSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Color(glm::vec4(0.5647f, 0.933f, 0.5647f, 0.6f));
						crosshair->getComponent<components::Crosshair>()->bulletType = components::BULLET_TYPE::FIRE;
						break;
					case 1:
						iceSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Color(glm::vec4(0.5647f, 0.933f, 0.5647f, 0.6f));
						fireSpellEntity->getComponent<components::Sprite>()->spriteColor = Ebony::Colors::White;
						crosshair->getComponent<components::Crosshair>()->bulletType = components::BULLET_TYPE::ICE;
						break;
					}

				} });





			Ebony::SystemManager::AddEntity(fireSpellEntity);
			Ebony::SystemManager::AddEntity(iceSpellEntity);

			entity->addComponent(std::make_unique<components::Transform>(initialUIPlacement, 0.0f, sprite->GetDimensions() * overallScaling));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(selectedSpellComponent));
			entity->addComponent(std::move(keyboardComponent));
			entity->addComponent(std::move(controllerComponent));

			return entity;
		}
	};
}