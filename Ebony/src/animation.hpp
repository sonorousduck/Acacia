#pragma once

#include "spritesheet.hpp"
#include <functional>
#include <chrono>
#include "colors.hpp"
#include <iostream>
#include <optional>
#include <map>

namespace Ebony
{
	class Animation
	{
	public:
		Animation(SpriteSheet spritesheet) : m_Spritesheet(spritesheet), depth(1.0) {};
		Animation(SpriteSheet spritesheet, bool repeatForever) : m_Spritesheet(spritesheet), depth(1.0), m_ShouldRepeatForever(repeatForever) {};

		const auto& GetSprite() const { return m_Spritesheet; }
		auto GetSpriteCount() const { return m_Spritesheet.numDivisions; }
		auto GetSpriteTime() const { return m_Spritesheet.timings[m_CurrentSprite]; }
		auto GetSpriteColor() const { return m_SpriteColor; }
		auto GetCurrentSpriteFrame() const { return m_CurrentSprite; }
		//auto GetCurrentFrame() { return m_FrameNumber; }
		auto GetCurrentSpriteRect()
		{
			std::cout << "Needs to be implemented" << std::endl;
			return;
		}


		void IncrementSprite() 
		{ 
			if (m_CurrentSprite + 1 < m_Spritesheet.numDivisions)
			{
				m_CurrentSprite++;
			}
			else if ((m_ShouldRepeatForever || m_NumRepeatTimes > 0))
			{
				m_CurrentSprite = (m_CurrentSprite + 1) % m_Spritesheet.numDivisions;
			}
		}
		//void IncrementFrameNumber() { m_FrameNumber = (m_FrameNumber + 1); }
		void ResetAnimation()
		{
			m_CurrentSprite = 0;
			m_FrameNumber = 0;
			m_ElapsedTime = std::chrono::microseconds::zero();
		}

		auto getElapsedTime() { return m_ElapsedTime; }
		void updateElapsedTime(std::chrono::microseconds time) 
		{ 
			m_ElapsedTime += time;
			while (m_ElapsedTime >= GetSpriteTime())
			{
				m_ElapsedTime -= GetSpriteTime();
				IncrementSprite();
			}
		
		}

		void SetDepth(float newDepth)
		{
			depth = newDepth;
		}

		auto GetDepth()
		{
			return depth;
		}

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

		std::map<std::uint16_t, std::vector<std::function<void()>>> onAnimationFrame; // <Frame number, function>

	private:
		SpriteSheet m_Spritesheet;

		std::uint16_t m_CurrentSprite{ 0 };
		std::uint16_t m_NumRepeatTimes{ 0 };
		std::uint16_t m_FrameNumber{ 0 };
		float depth = 1.0;
		bool m_ShouldRepeatForever{ false };
		bool m_Started{ false };




		std::chrono::microseconds m_ElapsedTime{ 0 };
		Color m_SpriteColor{ Colors::White };
	};
}
