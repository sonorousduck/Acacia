#pragma once

#include "Core.hpp"
#include "events/Event.hpp"
#include "events/ApplicationEvent.hpp"
#include "Log.hpp"
#include <chrono>
#include <iostream>
#include <graphics2d.hpp>

namespace Ebony {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		virtual void Run();
		virtual void Init();
		virtual void AddNewEntities();
		virtual void RemoveOldEntities();
		virtual void Update(std::chrono::microseconds elapsedTime);
		virtual void Draw(std::chrono::microseconds elapsedTime);
		virtual void ProcessInput(std::chrono::microseconds elapsedTime);
		
		
	
	};

	// To be defined in a client
	Application* CreateApplication();
}