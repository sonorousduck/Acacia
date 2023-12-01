#pragma once

#include "entity.hpp"
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include <misc/resourceManager.hpp>
#include "../misc/collisionLayers.hpp"
#include "../components/powerupComponent.hpp"





namespace BrickBreaker
{
	class Powerup
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, std::uint8_t powerupType, const char* sprite, std::function<void(entities::Entity::IdType)> RemoveEntity)
		{
			entities::EntityPtr powerup = std::make_shared<entities::Entity>();
			powerup->addComponent(std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(25.0f, 25.0f)));
			powerup->addComponent(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(sprite), Ebony::Colors::White));
			powerup->addComponent(std::make_unique<components::Powerup>(Powerups::LARGER_PADDLE));
			powerup->addComponent(std::make_unique<components::RigidBody>(glm::vec2(0.0f, 250.0f), glm::vec2(0.0f, 0.0f), false));

			components::Subcollider aabbCollider = components::Subcollider(glm::vec2(13.5f, 13.5f), glm::vec2(25.0f, 25.0f), true, true);
			aabbCollider.onCollisionStart = [=](entities::EntityPtr self, entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					BrickBreaker::CollisionLayers layer = CollisionLayers(other->getComponent<components::Collider>()->layer);

					if (layer & BrickBreaker::CollisionLayers::PADDLE)
					{
						// Confer powers upon the user	
						auto powerupComponent = self->getComponent<components::Powerup>();
						
						//other->getComponent<components::SoundEffect>()->soundEffectQueue.push_back(Ebony::ResourceManager::GetSoundEffect("powerup"));

						if (powerupComponent->powerupType & Powerups::LARGER_PADDLE)
						{
							auto transform = other->getComponent<components::Transform>();
							auto collider = other->getComponent<components::Collider>();

							//if (transform->scale.x == 150.0f)
							//{
								transform->scale.x += 50.0f;
								collider->aabbCollider.changeCenter(collider->aabbCollider.getCenter() + glm::vec2(25.0f, 0.0f));
								collider->aabbCollider.changeSize(collider->aabbCollider.getSize() + glm::vec2(50.0f, 0.0f));
							//}
						}
						else if (powerupComponent->powerupType & Powerups::ADDITIONAL_BALL)
						{


						}
						else if (powerupComponent->powerupType & Powerups::SLOWMO_REFILL)
						{


						}


						RemoveEntity(self->getId());
					}

				};
			
			auto collider = std::make_unique<components::Collider>(aabbCollider, BrickBreaker::CollisionLayers::POWERUP, false);

			



			powerup->addComponent(std::move(collider));

			return powerup;
		}
	};
}