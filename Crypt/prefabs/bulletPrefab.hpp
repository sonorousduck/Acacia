#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <graphics2d.hpp>
#include <SDL.h>
#include <components/controllerComponent.hpp>
#include <components/keyboardComponent.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/destructionComponent.hpp>
#include <components/timedComponent.hpp>

#include <singletons/time.hpp>
#include <components/mouseInputComponent.hpp>
#include <components/animationControllerComponent.hpp>
#include "../components/bulletComponent.hpp"
#include "../components/aimComponent.hpp"
#include <iostream>
#include <components/soundEffectComponent.hpp>
#include <singletons/audioManager.hpp>
#include <misc/renderLayers.hpp>

#include <components/aiComponent.hpp>
#include "../misc/aiInformationTypes.hpp"


namespace Crypt
{
	class BulletPrefab
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale, glm::vec2 direction, float speed, components::BULLET_TYPE bulletType, std::uint8_t strength, const char* texture, float timeUntilDestruction = 0.25f, std::uint16_t collisionLayer = Crypt::CollisionLayers::PLAYER_BULLET, std::uint16_t layersToCollideWith = Crypt::CollisionLayers::ENEMY | Crypt::CollisionLayers::GROUND, float transformModification = 0.0f)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(texture), Ebony::Colors::White, Ebony::RenderLayer::FOREGROUND);
			auto spriteScale = sprite->GetDimensions();

			components::Subcollider aabbcollider = components::Subcollider((spriteScale * scale) / 2.0f, (spriteScale * scale), true, true);

			aabbcollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					auto currentLocation = entity->getComponent<components::Transform>();
					if (startTransform.x + 20.0f >= currentLocation->position.x && startTransform.x - 20.0f <= currentLocation->position.x
						&& startTransform.y + 20.0f >= currentLocation->position.y && startTransform.y - 20.0f <= currentLocation->position.y)
					{
						return;
					}

					if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::GROUND)
					{					
						entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
					}

					if (other->getComponent<components::Collider>()->layer & Crypt::CollisionLayers::PLAYER)
					{
						entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
						// TODO: Add particle effects entity here!
						//AddEntity()
					}
				};


			auto collider = std::make_unique<components::Collider>(aabbcollider, collisionLayer, layersToCollideWith, false);
			auto transform = std::make_unique<components::Transform>(startTransform, ((glm::atan(direction.y, direction.x)) / (2 * glm::pi<float>()) * 360.0f) + transformModification, (spriteScale * scale));
			auto rigidbody = std::make_unique<components::RigidBody>();

			auto soundeffects = std::make_unique<components::SoundEffect>(Ebony::AudioType::ENTITY);

			soundeffects->soundEffectQueue.push_back(Ebony::IndividualSound(Ebony::ResourceManager::GetSoundEffect("spell"), 2));

			rigidbody->setVelocity(direction * speed);



			entity->addComponent(std::make_unique<components::TimedComponent>(timeUntilDestruction, [=]() 
				{
					entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;				
				}));
			entity->addComponent(std::make_unique<components::DestructionComponent>([=]() 
				{
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));


			// Only care about sending it to the AI if it is part of the enemies bullets, not your own
			if (collisionLayer != Crypt::CollisionLayers::PLAYER_BULLET)
			{
				entity->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE, Crypt::AiInformationTypes::BULLET_INFORMATION));
			}


			entity->addComponent(std::make_unique<components::Bullet>(bulletType, strength));
			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(rigidbody));
			entity->addComponent(std::move(soundeffects));

			return entity;
		}

	};
}