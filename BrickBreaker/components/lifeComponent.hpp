#pragma once

#include <components/component.hpp>

// Empty component denoting it contains the life text. GameManager handles lives for me
namespace components
{
	class Life : public PolymorphicComparable<Component, Life>
	{
	public:
		Life()
		{}

	};
}