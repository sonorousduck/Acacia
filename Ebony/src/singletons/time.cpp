#include "time.hpp"

namespace Ebony 
{

	std::chrono::microseconds Time::elapsedTime{};

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
		elapsedTime = newElapsedTime;
	}

}
