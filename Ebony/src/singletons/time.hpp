#pragma once

#include <chrono>

namespace Ebony
{

	class Time
	{
	public:
		static std::chrono::microseconds GetDeltaTime();
		static float GetDeltaTimeAsFloat();
		static float GetDeltaTimeFloatMilli();
		static void SetDeltaTime(std::chrono::microseconds newElapsedTime);
	private:
		Time() {};

		static std::chrono::microseconds elapsedTime;

	};

}