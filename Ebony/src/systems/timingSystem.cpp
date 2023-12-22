#include "timingSystem.hpp"
#include "../singletons/time.hpp"


namespace systems
{
	void TimingSystem::Update(std::chrono::microseconds elapsedTime)
	{
		this->updateImpl(elapsedTime);
	}

	void TimingSystem::updateImpl(std::chrono::microseconds elapsedTime)
	{
		for (auto& [id, entity] : m_Entities)
		{
			auto timedComponent = entity->getComponent<components::TimedComponent>();

			if (timedComponent->currentTime > 0.0f && timedComponent->running)
			{
				timedComponent->currentTime -= Ebony::Time::GetDeltaTimeFloat();

				if (timedComponent->currentTime <= 0.0f)
				{
					timedComponent->timerExpiredCallback();
				}
			}
		}
	}
}