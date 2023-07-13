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

	void Application::Init()
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

		while (true)
		{
			ProcessInput();
			Update();
			Draw();
		}
	}

}
