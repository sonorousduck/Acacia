#pragma once

#include "component.hpp"
#include"animation.hpp"
#include <chrono>
#include <cstdint>
#include <memory>
#include <vector>


namespace components
{
	using namespace Ebony;

	class AnimationController : public PolymorphicComparable<Component, AnimationController>
	{
	public:
		AnimationController();
		AnimationController(std::vector<Animation> animations);


		void AddAnimation(Animation animation) { animations.emplace_back(animation); }

        virtual std::tuple<ctti::unnamed_type_id_t, std::unique_ptr<Component>> clone() override
		{
			return { ctti::unnamed_type_id<AnimationController>(), std::make_unique<AnimationController>(animations) };
		}

		bool operator==(AnimationController& rhs)
		{
			if (animations.size() == rhs.animations.size())
			{
				for (unsigned int i = 0; i < animations.size(); i++)
				{
					if (animations[i] != rhs.animations[i])
					{
						return false;
					}
				}

				return true;

			}
			return false;
		}



		std::vector<Animation> animations;

	};
}

