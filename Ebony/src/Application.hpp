#pragma once

#include "Core.hpp"
#include "events/Event.hpp"
#include "events/ApplicationEvent.hpp"
#include "Log.hpp"

#include <iostream>

namespace Ebony {
	class EBONY_API Application
	{
	public:
		Application();
		virtual ~Application();
		void Run();
	};

	// To be defined in a client
	Application* CreateApplication();
}