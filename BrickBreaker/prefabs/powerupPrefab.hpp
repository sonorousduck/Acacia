#pragma once

#include "entity.hpp"
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include <misc/resourceManager.hpp>
#include "../misc/collisionLayers.hpp"
#include "../components/powerupComponent.hpp"
#include <misc/renderLayers.hpp>
#include <singletons/systemManager.hpp>



namespace BrickBreaker
{
	class Powerup
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, std::uint8_t powerupType, const char* spriteName)
		{
			entities::EntityPtr powerup = std::make_shared<entities::Entity>();
			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(spriteName), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto scale = sprite->GetDimensions();
			powerup->addComponent(std::make_unique<components::Transform>(startTransform, 0.0f, scale));
			powerup->addComponent(std::move(sprite));
			powerup->addComponent(std::make_unique<components::Powerup>(Powerups::LARGER_PADDLE));
			powerup->addComponent(std::make_unique<components::RigidBody>(glm::vec2(0.0f, 250.0f), glm::vec2(0.0f, 0.0f), false));

			components::Subcollider aabbCollider = components::Subcollider(scale / 2.0f, scale, true, true);
			aabbCollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					if (layer & BrickBreaker::CollisionLayers::PADDLE)
					{
						// Confer powers upon the user	
						auto powerupComponent = powerup->getComponent<components::Powerup>();
						
						//other->getComponent<components::SoundEffect>()->soundEffectQueue.push_back(Ebony::ResourceManager::GetSoundEffect("powerup"));

						if (powerupComponent->powerupType & Powerups::LARGER_PADDLE)
						{
							auto transform = other->getComponent<components::Transform>();
							auto collider = other->getComponent<components::Collider>();

							if (transform->scale.x == 48.0f)
							{
								transform->scale.x += 16.0f;

								collider->aabbCollider.changeCenter(collider->aabbCollider.getCenter() + glm::vec2(8.0f, 0.0f));
								collider->aabbCollider.changeSize(collider->aabbCollider.getSize() + glm::vec2(16.0f, 0.0f));
							}

						}
						else if (powerupComponent->powerupType & Powerups::ADDITIONAL_BALL)
						{


						}
						else if (powerupComponent->powerupType & Powerups::SLOWMO_REFILL)
						{


						}


						Ebony::SystemManager::RemoveEntity(powerup->getId());
					}

				};
			
			auto collider = std::make_unique<components::Collider>(aabbCollider, BrickBreaker::CollisionLayers::POWERUP, false);

			



			powerup->addComponent(std::move(collider));

			return powerup;
		}
	};
}