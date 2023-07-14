#pragma once

#include "spritesheet.hpp"
#include <functional>
#include <chrono>
#include "colors.hpp"
#include <iostream>
#include <optional>

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


		void IncrementSprite() { m_CurrentSprite = (m_CurrentSprite + 1) % m_Spritesheet.numDivisions; }
		void ResetAnimation()
		{
			m_CurrentSprite = 0;
			m_ElapsedTime = std::chrono::microseconds::zero();
		}

		auto getElapsedTime() { return m_ElapsedTime; }
		void updateElapsedTime(std::chrono::microseconds time) { m_ElapsedTime += time; }

		void AddOnAnimationStart()
		{

		}

		void AddOnAnimationEnd()
		{

		}

		void StartAnimation()
		{
			m_Started = true;
		}


		bool operator==(Animation& rhs)
		{
			return (m_Spritesheet == rhs.m_Spritesheet
				&& m_NumRepeatTimes == rhs.m_NumRepeatTimes
				&& GetSpriteTime() == rhs.GetSpriteTime()
				&& m_SpriteColor == rhs.m_SpriteColor);
		}

		std::vector<std::pair<int, std::function<void()>>> onAnimationFrame; // <Frame number, function>

	private:
		SpriteSheet m_Spritesheet;

		std::uint8_t m_CurrentSprite{ 0 };
		std::uint8_t m_NumRepeatTimes{ 0 };
		bool m_ShouldRepeatForever{ false };
		bool m_Started{ false };




		std::chrono::microseconds m_ElapsedTime{ 0 };
		Color m_SpriteColor{ Colors::White };
	};
}
