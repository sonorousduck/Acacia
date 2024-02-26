#pragma once

#include <chrono>

namespace Ebony
{

	class Time
	{
	public:
		static std::chrono::microseconds GetDeltaTime();
		static float GetDeltaTimeAsFloatMicroseconds();
		static float GetDeltaTimeFloat();
		static void SetDeltaTime(std::chrono::microseconds newElapsedTime);
		static void ResetTotalElapsedTime();
		static float GetTotalElapsedTime();
	private:
		Time() {};

		static std::chrono::microseconds elapsedTime;
		static std::chrono::microseconds totalElapsedTime;

	};

}