#pragma once

#include "Core.hpp"

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