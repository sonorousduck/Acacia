#pragma once
#include <entity.hpp>

#include <glm/glm.hpp>

#include <components/component.hpp>
#include <random>
#include <numbers>

#include <Core.hpp>

namespace components
{
	class EnemyDetection : public PolymorphicComparable<Component, EnemyDetection>
	{
	public:
		EnemyDetection() : detectionRange(0.0f), movementRange(0.0f), movementSpeed(0.0f), target(nullptr)
		{}

		EnemyDetection(float detectionRange, float movementRange, float movementSpeed, glm::vec2 offset, entities::EntityPtr target) : 
			detectionRange(detectionRange), 
			movementRange(movementRange), 
			movementSpeed(movementSpeed),
			offset(offset),
			target(target)
		{}


		float detectionRange;
		float movementRange;
		float movementSpeed;
		entities::EntityPtr target;
		bool canDetectTarget = false;
		bool canMoveTowardsTarget = false;
		glm::vec2 offset{ 0.0f, 0.0f };

		glm::vec2 towardsTargetVector{0.0f, 0.0f};



	};
}
