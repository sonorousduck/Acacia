#pragma once

#include <vector>

#include "component.hpp"
#include "subcolliderClass.hpp"
#include <set>

namespace components
{
	class Collider : public PolymorphicComparable<Component, Collider>
	{
	public:
		Collider(Subcollider aabbCollider, std::uint16_t layer, bool collidesWithOwnLayer = true) : aabbCollider(aabbCollider), layer(layer), collidesWithOwnLayer(collidesWithOwnLayer) {};

		Collider(Subcollider aabbCollider, std::vector<Subcollider> subcolliders, std::uint16_t layer, bool collidesWithOwnLayer = true) : aabbCollider(aabbCollider), subcolliders(subcolliders), preciseSubcolliderDetection(true), layer(layer), collidesWithOwnLayer(collidesWithOwnLayer) {}


		Subcollider aabbCollider;
		std::vector<Subcollider> subcolliders{};

		bool preciseSubcolliderDetection = false;
		bool isEnabled = true;
		bool isCollidingLastFrame = false;
		bool isDebugging = true;
		// This is to allow a user to specify if its own layer can collide. I.E. In brick breaker, the bricks are on the collision layer of "BRICK" but you don't want to check collisions against other bricks
		bool collidesWithOwnLayer;

		std::set<std::uint16_t> currentlyCollidingWith{};

		// Need to figure out how to do the bitwise comparisons
		std::uint16_t layer;
		
		

	private:



	};
}

