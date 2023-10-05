#pragma once

#include <vector>

#include "component.hpp"
#include "subcolliderClass.hpp"

namespace components
{
	class Collider : public PolymorphicComparable<Component, Collider>
	{
	public:
		Collider(Subcollider aabbCollider) : aabbCollider(aabbCollider) {};

		Collider(Subcollider aabbCollider, std::vector<Subcollider> subcolliders) : aabbCollider(aabbCollider), subcolliders(subcolliders), preciseSubcolliderDetection(true) {}


		Subcollider aabbCollider;
		std::vector<Subcollider> subcolliders{};

		bool preciseSubcolliderDetection = false;
		bool isEnabled = true;

		// Need to figure out how to do the bitwise comparisons
		std::uint16_t layer;
		

	private:



	};
}

