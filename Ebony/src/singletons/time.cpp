#include "time.hpp"

namespace Ebony 
{

	std::chrono::microseconds Time::elapsedTime{};
	std::chrono::microseconds Time::totalElapsedTime{};

	std::chrono::microseconds Time::GetDeltaTime()
	{
		return elapsedTime;
	}

	float Time::GetDeltaTimeAsFloatMicroseconds()
	{
		return static_cast<float>(elapsedTime.count());
	}

	float Time::GetDeltaTimeFloat()
	{
		return static_cast<float>(elapsedTime.count()) / 1000000.0f;
	}

	void Time::SetDeltaTime(std::chrono::microseconds newElapsedTime)
	{
		totalElapsedTime += newElapsedTime;
		elapsedTime = newElapsedTime;
	}

	void Time::ResetTotalElapsedTime()
	{
		totalElapsedTime = std::chrono::microseconds(0);
	}

	float Time::GetTotalElapsedTime()
	{
		return static_cast<float>(totalElapsedTime.count()) / 1000000.0f;
	}

}
