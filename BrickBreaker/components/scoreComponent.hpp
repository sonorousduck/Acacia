#pragma once

#include <components/component.hpp>

// Empty component denoting it contains the score text. GameManager handles lives for me
namespace components
{
	class Score : public PolymorphicComparable<Component, Score>
	{
	public:
		Score()
		{}

	};
}