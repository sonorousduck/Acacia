#pragma once

#include <optional>
#include <functional>
#include <deque>

#include "component.hpp"
#include "glm/glm.hpp"


namespace components
{
	class RigidBody : public PolymorphicComparable<Component, RigidBody>
	{
	public:
		RigidBody(glm::vec2 position) : 
			position(position),
			previousPosition(position) 
		{}
		
		RigidBody(glm::vec2 position, glm::vec2 velocity, glm::vec2 acceleration, bool usesGravity = false) : 
			position(position), 
			previousPosition(position), 
			velocity(velocity), 
			acceleration(acceleration), 
			usesGravity(usesGravity) 
		{}


		glm::vec2 getPreviousPosition() { return this->previousPosition; }
		glm::vec2 getPosition() { return this->position; }
		glm::vec2 getVelocity() { return this->velocity; }
		glm::vec2 getAcceleration() { return this->acceleration; }
		
		// Really, I would only want the Physics System to change these. Maybe look into a way to declare the physics system a friend?
		void setPreviousPosition(glm::vec2 value) { this->previousPosition = value; }
		void setPosition(glm::vec2 value) { this->position = value; }
		void setVelocity(glm::vec2 value) { this->velocity = value; }
		void setAcceleration(glm::vec2 value) { this->acceleration = value; }

		// Enables gravity calculations
		bool usesGravity = false;

		bool checkHasMoved() { return hasMoved; }
		void toggleHasMoved() { this->hasMoved = !this->hasMoved; }



		// Returns the next scripted movement and removes it from the list
		glm::vec2 getNextScriptedMovement() 
		{ 
			glm::vec2 nextMovement = scriptedMovements.back();
			scriptedMovements.pop_back(); 
			
			return nextMovement;
		}

		// Adds a scripted movement to the queue. Expects it as a glm::vec2
		void addScriptedMovement(glm::vec2 scriptedMovement)
		{
			scriptedMovements.push_back(scriptedMovement);
		}

		std::uint16_t getNextScriptedMovementLength()
		{
			return static_cast<std::uint16_t>(scriptedMovements.size());
		}

		// Returns the next force and removes it from the list
		glm::vec2 getNextForce()
		{
			glm::vec2 nextForce = forces.back();
			forces.pop_back();

			return nextForce;
		}

		// Adds a force to the queue. Expects it as a glm::vec2
		void addForce(glm::vec2 force)
		{
			forces.push_back(force);
		}

		std::uint16_t getForceLength()
		{
			return static_cast<std::uint16_t>(forces.size());
		}

		


		// TODO: Add the collider once it is defined here. Though, maybe not. I don't know if components should have components...


	private:
		
		glm::vec2 previousPosition{};
		glm::vec2 position{};
		glm::vec2 velocity{};
		glm::vec2 acceleration{};
		


		// Indicates that it has moved this frame. Useful for quick checking to see if it should even check for collisions
		// If something moved into it though, the other object would be marked as for detection and would detect this.
		bool hasMoved = false;

		// This exposes a way for a script or another system to "set" the position of the rigidbody
		// This is the safe way to instantly modify where the object is placed. 
		std::vector<glm::vec2> scriptedMovements{};

		// If you want to apply pushes in any direction, this is where you will add to.
		std::vector<glm::vec2> forces{};


	};
}