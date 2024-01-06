#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>

#include <components/rigidbodyComponent.hpp>
#include <singletons/time.hpp>
#include <components/animationControllerComponent.hpp>
#include "../components/enemyDetectionComponent.hpp"
#include "../components/playerComponent.hpp"
#include "../components/enemyComponent.hpp"

#include <components/cppScriptComponent.hpp>
#include "healthBarPrefab.hpp"
#include "../scripts/suicideBatScript.hpp"


namespace Crypt
{
	class ExplosionDeathPrefab
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform, glm::vec2 scale)
		{
			entities::EntityPtr explosion = std::make_shared<entities::Entity>();

			auto animationController = std::make_unique<components::AnimationController>();

			auto spriteSheet = Ebony::ResourceManager::GetTexture("explosion");

			std::vector<std::chrono::microseconds> timings(12, std::chrono::milliseconds(75));

			std::vector<components::Link> links = { components::Link(1, [=]() {
								return (false);

			}) };
			
			auto animation = Ebony::Animation(SpriteSheet(spriteSheet, 12, timings));


			animation.onAnimationFrame.insert({ 11, std::vector<std::function<void()>>
			{
				[=]()
				{
					explosion->getComponent<components::DestructionComponent>()->shouldDestroy = true;
				},
			} });

			std::vector<Ebony::Animation> animations = { animation };


			auto node1 = components::Node(links, animations);

			animationController->animationTree.emplace_back(node1);
			explosion->addComponent(std::move(animationController));


			scale *= glm::vec2(spriteSheet->Width / 12, spriteSheet->Height); // We know there are 12 in the x


			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, scale);
			

			explosion->addComponent(std::make_unique<components::DestructionComponent>([=]()
				{
					// Eventually, this is where the animations will be inserted and played probably (Maybe just spawn an entity or something)
					explosion->getComponent<components::DestructionComponent>()->shouldRemove = true;
				}));

			explosion->addComponent(std::move(transform));
			//bird->addComponent(std::move(sprite));
			

			return explosion;

		}
	};
}