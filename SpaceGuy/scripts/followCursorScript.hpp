#pragma once
#include <components/rigidbodyComponent.hpp>

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>
#include <components/text.hpp>

namespace scripts
{
	struct FollowCursorScript : components::CppScript
	{
	public:
		FollowCursorScript(entities::EntityPtr parent) : parent(parent)
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{
			// Get if we can move, then if we can shoot
			auto cursorPosition = parent->getComponent<components::Transform>();
			auto sprite = entity->getComponent<components::Sprite>();

			glm::vec2 centerPosition = cursorPosition->position - (sprite->GetDimensions() / 2.0f);

			entity->getComponent<components::Transform>()->position = centerPosition;


			// If valid
			entity->getComponent<components::Sprite>()->spriteColor = Ebony::Colors::Green;


		}

		entities::EntityPtr parent;

	};
}