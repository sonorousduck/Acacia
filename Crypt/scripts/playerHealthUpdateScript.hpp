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
		PlayerHealthUpdateScript(entities::EntityPtr parent, entities::EntityPtr heart1, entities::EntityPtr heart2, entities::EntityPtr heart3) : parent(parent), heart1(heart1), heart2(heart2), heart3(heart3)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto player = parent->getComponent<components::Player>();
			auto playerHealth = player->health;
			auto lastHealth = player->lastHealth;


			if (lastHealth != playerHealth)
			{
				switch (playerHealth)
				{
				case 0:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					if (Ebony::SystemManager::aiEnabled)
					{
						Ebony::SystemManager::shouldResetForAi = true;
					}
					else
					{
						Ebony::SystemManager::nextScreenEnum = Crypt::ScreenEnum::GAME_OVER;
					}
					break;
				case 1:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("damaged_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					break;
				case 2:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					break;
				case 3:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("damaged_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");

					break;
				case 4:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("broken_heart");
					break;
				case 5:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("damaged_heart");
					break;
				case 6:
					heart1->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart2->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					heart3->getComponent<components::Sprite>()->texture = Ebony::ResourceManager::GetTexture("full_heart");
					break;
				}


				lastHealth = playerHealth;
			}

			if (playerHealth < 0)
			{
				if (Ebony::SystemManager::aiEnabled)
				{
					Ebony::SystemManager::shouldResetForAi = true;
				}
				else
				{
					Ebony::SystemManager::nextScreenEnum = Crypt::ScreenEnum::GAME_OVER;
				}
			}
		}

		entities::EntityPtr parent;
		entities::EntityPtr heart1;
		entities::EntityPtr heart2;
		entities::EntityPtr heart3;

	};
}