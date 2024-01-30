#pragma once
#include <entity.hpp>
#include <components/transform.hpp>
#include <components/sprite.hpp>
#include <misc/resourceManager.hpp>
#include <components/subcolliderClass.hpp>
#include "../misc/collisionLayers.hpp"
#include <components/collider.hpp>
#include <components/rigidbodyComponent.hpp>
#include <components/cppScriptComponent.hpp>
#include "../scripts/DeathScript.hpp"


namespace BrickBreaker
{
	class Wall
	{
	public:
		static entities::EntityPtr Create(float transformWidth, float transformHeight, float scaleX, float scaleY, BrickBreaker::CollisionLayers collisionType, bool isBottomWall = false)
		{
			entities::EntityPtr wall = std::make_shared<entities::Entity>();

			auto sprite = std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("default"), Ebony::Colors::Red, Ebony::ALWAYS_FRONT);

			wall->addComponent(std::move(std::make_unique<components::Transform>(glm::vec2(transformWidth, transformHeight), 0.0f, glm::vec2(scaleX, scaleY))));
			components::Subcollider wallCollider = components::Subcollider(glm::vec2(scaleX / 2.0f, scaleY / 2.0f), glm::vec2(scaleX, scaleY), true, true);
			wall->addComponent(std::move(std::make_unique<components::Collider>(wallCollider, collisionType, BrickBreaker::CollisionLayers::BALL, true)));
			wall->addComponent(std::move(std::make_unique<components::RigidBody>()));
			//wall->addComponent(std::move(std::make_unique<components::Sprite>(Ebony::ResourceManager::GetShader("default"), Ebony::ResourceManager::GetTexture("empty"), Ebony::Colors::White)));
			wall->addComponent(std::move(sprite));

			if (isBottomWall)
			{
				std::unique_ptr<components::CppScript> script = std::make_unique<scripts::DeathScript>();

				wall->addComponent(std::move(script));
			}


			return wall;
		}

	};
}