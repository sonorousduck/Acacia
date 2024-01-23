#pragma once

#include "../Ebony/src/components/cppScriptComponent.hpp"
#include <components/transform.hpp>
#include <singletons/time.hpp>

#include <graphics2d.hpp>

namespace scripts
{
	struct FollowPlayerCamera : components::CppScript
	{
	public:
		FollowPlayerCamera()
		{
		}


		void Update(std::chrono::microseconds elapsedTime) override
		{	
			auto transform = entity->getComponent<components::Transform>();

			Ebony::Graphics2d::mainCamera->SetCameraPosition(glm::vec2(Ebony::Graphics2d::mainCamera->Position) + transform->position - transform->previousPosition);

		}
	};
}