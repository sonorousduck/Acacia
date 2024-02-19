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
#include "../components/playerInformation.hpp"

namespace SpaceGuy
{
	class EnemyBullet
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, float angle, float bulletStrength, const char* spriteImage, const char* soundeffect, glm::vec2 bulletScale, bool playSound = true)
		{
			auto bulletSpeed = 200.0f;
			auto timeUntilDestruction = 5.0f;

			entities::EntityPtr entity = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture(spriteImage), Ebony::Colors::White, Ebony::FOREGROUND);
			auto scale = sprite->GetDimensions() * bulletScale;

			components::Subcollider aabbCollider = components::Subcollider(scale / 2.0f, scale, true, true);

			aabbCollider.onCollisionStart = [=](entities::EntityPtr other, std::chrono::microseconds elapsedTime)
				{
					entity->getComponent<components::DestructionComponent>()->shouldDestroy = true;

					components::PlayerInformation* playerInformation;
					if (other->tryGetComponent(playerInformation))
					{
						playerInformation->health -= entity->getComponent<components::Bullet>()->strength;
					}

				};

			auto collider = std::make_unique<components::Collider>(aabbCollider, SpaceGuy::CollisionLayers::ENEMY_BULLET, SpaceGuy::CollisionLayers::PLAYER | SpaceGuy::CollisionLayers::WALL, false);
			auto transform = std::make_unique<components::Transform>(startTransform, angle, scale);
			auto rigidbody = std::make_unique<components::RigidBody>();


			if (playSound)
			{
				auto soundeffects = std::make_unique<components::SoundEffect>(Ebony::AudioType::ENTITY);
				soundeffects->soundEffectQueue.push_back(Ebony::IndividualSound(Ebony::ResourceManager::GetSoundEffect(soundeffect), 90));
				entity->addComponent(std::move(soundeffects));
			}

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

			entity->addComponent(std::move(transform));
			entity->addComponent(std::move(sprite));
			entity->addComponent(std::move(collider));
			entity->addComponent(std::move(rigidbody));
			entity->addComponent(std::make_unique<components::Bullet>(bulletStrength));
			entity->addComponent(std::make_unique<components::AIComponent>(Ebony::AIType::STATE, SpaceGuy::AiInformationTypes::ENEMY_BULLET_INFORMATION));

			return entity;

		}
	};
}