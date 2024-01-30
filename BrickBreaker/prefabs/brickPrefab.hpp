#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include "../components/ballComponent.hpp"
#include "../components/brickComponent.hpp"
#include "../prefabs/powerupPrefab.hpp"
#include "../singletons/GameManager.hpp"
#include <components/soundEffectComponent.hpp>
#include <misc/renderLayers.hpp>
#include <singletons/systemManager.hpp>
#include "../singletons/GameManager.hpp"

#include <components/aiComponent.hpp>
#include "../misc/aiInformationTypes.hpp"



namespace BrickBreaker
{
	class Brick
	{
	public:
		static entities::EntityPtr Create(float transformWidth, float transformHeight, const char* tile_image, int brickStrength, std::uint32_t pointValue)
		{
			entities::EntityPtr brickEntity = std::make_shared<entities::Entity>();
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(tile_image), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto scale = sprite->GetDimensions();

			brickEntity->addComponent(std::make_unique<components::Transform>(glm::vec2(transformWidth, transformHeight), 0.0f, scale));
			components::Subcollider subcollider = components::Subcollider(scale / 2.0f, scale, true, true);
			brickEntity->addComponent(std::make_unique<components::SoundEffect>(Ebony::ENTITY));

			subcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{

					components::Ball* ball;
					if (other->tryGetComponent(ball))
					{
						auto brick = brickEntity->getComponent<components::Brick>();

						brick->strength -= ball->strength;

						if (brick->strength <= 0)
						{
							// The brick should explode						
							other->getComponent<components::SoundEffect>()->soundEffectQueue.push_back(Ebony::IndividualSound(Ebony::ResourceManager::GetSoundEffect("brick_break"), 127));

							

							GameManager::addPoints(static_cast<float>(brick->pointValue));


							brick->destroyed = true;
							Ebony::SystemManager::RemoveEntity(brickEntity->getId());
							GameManager::brickCount--;

							if (GameManager::brickCount == 0)
							{
								GameManager::addPoints(1000.0f);
								Ebony::SystemManager::shouldResetForAi = true;
							}


							auto randomChance = rand() % 100;

							if (randomChance < 5)
							{
								// Spawn powerup entity
								Ebony::SystemManager::AddEntity(Powerup::Create(glm::vec2(transformWidth, transformHeight), Powerups::LARGER_PADDLE, "bigger_paddle_powerup"));
							}
						}
					}
				};

			brickEntity->addComponent(std::make_unique<components::Collider>(subcollider, BrickBreaker::CollisionLayers::BRICK, BrickBreaker::CollisionLayers::BALL, true));
			brickEntity->addComponent(std::make_unique<components::RigidBody>());
			brickEntity->addComponent(std::move(sprite));
			brickEntity->addComponent(std::make_unique<components::Brick>(brickStrength, pointValue));
			brickEntity->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE, BrickBreaker::AiInformationTypes::BRICK_INFORMATION));

			return brickEntity;
		}

	};
}