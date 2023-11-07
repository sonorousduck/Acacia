#include "time.hpp"

namespace Ebony 
{

	std::chrono::microseconds Time::elapsedTime{};

	std::chrono::microseconds Time::GetDeltaTime()
	{
		return elapsedTime;
	}

	float Time::GetDeltaTimeAsFloat()
	{
		return elapsedTime.count();
	}

	float Time::GetDeltaTimeFloatMilli()
	{
		return elapsedTime.count() / 1000.0f;
	}

	void Time::SetDeltaTime(std::chrono::microseconds newElapsedTime)
	{
		elapsedTime = newElapsedTime;
	}

}
