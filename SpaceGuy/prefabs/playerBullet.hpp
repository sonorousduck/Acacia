#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/collider.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/destructionComponent.hpp>
#include <components/timedComponent.hpp>

#include <singletons/time.hpp>
#include <misc/resourceManager.hpp>
#include <components/aiComponent.hpp>
#include "../components/bulletComponent.hpp"


namespace SpaceGuy
{
	class PlayerBullet
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, float angle, float bulletStrength, const char* spriteImage, const char* audioFile)
		{
			auto bulletSpeed = 200.0f;
			auto timeUntilDestruction = 5.0f;

			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(spriteImage), Ebony::Colors::White, Ebony::FOREGROUND);
			auto scale = sprite->GetDimensions();

			components::Subcollider aabbCollider = components::Subcollider(scale / 2.0f, scale, true, true);

			aabbCollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;				
				};



			auto collider = std::make_unique<components::Collider>(aabbCollider, SpaceGuy::CollisionLayers::PLAYER_BULLET, SpaceGuy::CollisionLayers::ENEMY | SpaceGuy::CollisionLayers::WALL, false);
			auto transform = std::make_unique<components::Transform>(startTransform, angle, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();

			auto soundeffects = std::make_unique<components::SoundEffect>(Ebony::AudioType::ENTITY);

			soundeffects->soundEffectQueue.push_back(Ebony::IndividualSound(Ebony::ResourceManager::GetSoundEffect(audioFile), 127));

			rigidbody->setVelocity(bulletSpeed * glm::vec2(glm::sin(glm::radians(angle)), -glm::cos(glm::radians(angle))));

			entity->addComponent(std::make_unique<components::TimedComponent>(timeUntilDestruction, [=]() 
				{
					entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;
				}));

			entity->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					entity->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			//entity->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE, Crypt::AiInformationTypes::BULLET_INFORMATION));

			entity->addComponent(std::make_unique<components::Bullet>(bulletStrength));
			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(rigidbody));
			entity->addComponent(std::move(soundeffects));

			return entity;

		}
	};
}