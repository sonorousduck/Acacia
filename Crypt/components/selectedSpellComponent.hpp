#pragma once

#include <glm/glm.hpp>
#include <components/component.hpp>

namespace components
{
	class SelectedSpell : public PolymorphicComparable<Component, SelectedSpell>
	{
	public:
		SelectedSpell() : selectedSpell(0)
		{}

		SelectedSpell(std::uint8_t selectedSpell) : selectedSpell(selectedSpell)
		{}


		std::uint8_t selectedSpell;
	};
}
