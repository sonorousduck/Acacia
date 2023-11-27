#pragma once
#include <cstdint>
#include <chrono>

namespace Ebony
{
	// This will be implemented by other games mostly, just not DEFAULT of 0. This is to let the 
	// screen declare the ScreenEnum type (Potentially circle back and just make this a std::uint16_t instead of the ScreenEnum)
	enum ScreenEnum : std::uint16_t
	{
		DEFAULT = 0,
	};


	class Screen
	{
	public:
		virtual void LoadContent() = 0;
		virtual void Run();
		virtual void Init();
		virtual void AddNewEntities();
		virtual void RemoveOldEntities();
		virtual void Update(std::chrono::microseconds elapsedTime);
		virtual void Draw(std::chrono::microseconds elapsedTime);
		virtual void ProcessInput(std::chrono::microseconds elapsedTime);
	};
}


