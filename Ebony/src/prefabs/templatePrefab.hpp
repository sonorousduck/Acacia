#pragma once

#include "../Ebony/src/entity.hpp"
#include <glm/glm.hpp>


namespace Ebony
{
	class Template
	{
	public:
		static entities::EntityPtr Create(glm::vec2 startTransform)
		{
			entities::EntityPtr entity = std::make_shared<entities::Entity>();


			/*auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::White);
			components::Subcollider aabbcollider = components::Subcollider(glm::vec2(windowWidth / 2.0f, 5.0f), glm::vec2(windowWidth, 10.0f), true, true);
			auto collider = std::make_unique<components::Collider>(aabbcollider, Crypt::CollisionLayers::GROUND, false);
			auto transform = std::make_unique<components::Transform>(startTransform, 0.0f, glm::vec2(windowWidth, 100.0f));
			auto rigidbody = std::make_unique<components::RigidBody>();



			ground->addComponent(std::move(collider));
			ground->addComponent(std::move(transform));
			ground->addComponent(std::move(sprite));
			ground->addComponent(std::move(rigidbody));*/

			return entity;
		}

	};
}