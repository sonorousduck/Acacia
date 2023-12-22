#pragma once

#include "component.hpp"


namespace components
{
	class TimedComponent : public PolymorphicComparable<Component, TimedComponent>
	{
	public:
		TimedComponent(float maxTime, std::function<void()> timerExpiredCallback) : maxTime(maxTime), currentTime(maxTime), timerExpiredCallback(timerExpiredCallback)
		{
		}


		void ResetTimer()
		{
			currentTime = maxTime;
		}

		float maxTime;
		float currentTime = 0.0f;
		bool running = true;
		std::function<void()> timerExpiredCallback;
	};
}