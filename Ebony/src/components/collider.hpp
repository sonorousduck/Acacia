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
		Collider(Subcollider aabbCollider, std::uint16_t layer, bool staticCollider) : aabbCollider(aabbCollider), layer(layer), layersToCollideWith(layer), isStatic(staticCollider) {};
		Collider(Subcollider aabbCollider, std::uint16_t layer, std::uint16_t layersToCollideWith, bool staticCollider) : aabbCollider(aabbCollider), layer(layer), layersToCollideWith(layersToCollideWith), isStatic(staticCollider) {};

		Collider(Subcollider aabbCollider, std::vector<Subcollider> subcolliders, std::uint16_t layer, bool staticCollider) : 
			aabbCollider(aabbCollider), 
			subcolliders(subcolliders), 
			preciseSubcolliderDetection(true), 
			layer(layer),
			layersToCollideWith(layer),
			isStatic(staticCollider)
		{}

		Collider(Subcollider aabbCollider, std::vector<Subcollider> subcolliders, std::uint16_t layer, std::uint16_t layersToCollideWith, bool staticCollider) :
		aabbCollider(aabbCollider),
		subcolliders(subcolliders),
		preciseSubcolliderDetection(true),
		layer(layer),
		layersToCollideWith(layersToCollideWith),
		isStatic(staticCollider)
	{}


		Subcollider aabbCollider;
		std::vector<Subcollider> subcolliders{};

		bool preciseSubcolliderDetection = false;
		bool isEnabled = true;
		bool isCollidingLastFrame = false;
		bool isDebugging = true;
		bool isStatic;

		std::set<std::uint16_t> currentlyCollidingWith{};

		std::uint16_t layer;
		std::uint16_t layersToCollideWith;
		

	private:



	};
}

