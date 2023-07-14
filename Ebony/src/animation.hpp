#pragma once

#include "spritesheet.hpp"
#include <functional>
#include <chrono>
#include "colors.hpp"
#include <iostream>

namespace Ebony
{
	class Animation
	{
	public:
		Animation(SpriteSheet spritesheet) : m_Spritesheet(spritesheet) {};

		auto GetSprite() { return m_Spritesheet; }
		auto GetSpriteCount() { return m_Spritesheet.numDivisions; }
		auto GetSpriteTime() { return m_Spritesheet.timings[m_CurrentSprite]; }
		auto GetSpriteColor() { return m_SpriteColor; }
		auto GetCurrentSprite() { return m_CurrentSprite; }
		auto GetCurrentSpriteRect()
		{
			std::cout << "Needs to be implemented" << std::endl;
			return;
		}


		void IncremenetSprite() { m_CurrentSprite = (m_CurrentSprite + 1) % m_Spritesheet.numDivisions; }
		void ResetAnimation()
		{
			m_CurrentSprite = 0;
			m_ElapsedTime = std::chrono::microseconds::zero();
		}

		auto getElapsedTime() { return m_ElapsedTime; }
		void updateElapsedTime(std::chrono::microseconds time) { m_ElapsedTime += time; }

		bool operator==(Animation& rhs)
		{
			return (m_Spritesheet == rhs.m_Spritesheet
				&& m_ShouldRepeat == rhs.m_ShouldRepeat
				&& m_NumRepeatTimes == rhs.m_NumRepeatTimes
				&& GetSpriteTime() == rhs.GetSpriteTime()
				&& m_SpriteColor == rhs.m_SpriteColor);
		}



		std::function<void*> onAnimationStart;
		std::function<void*> onAnimationUpdate;
		std::function<void*> onAnimationEnd;



	private:
		SpriteSheet m_Spritesheet;

		std::uint8_t m_CurrentSprite{ 0 };
		std::uint8_t m_NumRepeatTimes{ 0 };
		bool m_ShouldRepeat{ false };
		bool m_ShouldRepeatForever{ false };
		bool m_Started{ false };

		std::chrono::microseconds m_ElapsedTime{ 0 };
		Color m_SpriteColor{ Colors::White };
	};
}
