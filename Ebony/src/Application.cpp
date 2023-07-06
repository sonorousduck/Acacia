#include "Application.hpp"
#include "events/ApplicationEvent.hpp"
#include <sstream>
#include <iostream>

namespace Ebony {
	Application::Application()
	{

	}

	Application::~Application() 
	{

	}

	void Application::Update()
	{

	}

	void Application::Draw()
	{

	}

	void Application::ProcessInput()
	{

	}

	void Application::Run() 
	{
		WindowResizeEvent e(1280, 720);



		std::cout << "Beep" << std::endl;
		EB_TRACE(e);

		std::cout << test << std::endl;

		while (true)
		{
			ProcessInput();
			Update();
			Draw();
		}
	}

}
