#pragma once

#include "Core.hpp"
#include "events/Event.hpp"
#include "events/ApplicationEvent.hpp"
#include "Log.hpp"

#include <iostream>

namespace Ebony {
	class Application
	{
	public:
		int test;
		Application();
		virtual ~Application();
		void Run();
		virtual void Update();
		virtual void Draw();
		virtual void ProcessInput();
	};

	// To be defined in a client
	Application* CreateApplication();
}