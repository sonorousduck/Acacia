#include "Application.hpp"
//#include <sstream>
#include <iostream>
#include <SDL_mixer.h>

namespace Ebony {
	Application::Application()
	{

	}

	Application::~Application() 
	{

	}

	void Application::Init()
	{
		EB_TRACE("Running the parent's init function");

		if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		{
			std::cerr << "SDL audio could not initialize! SDL Error: " << Mix_GetError() << std::endl;
			exit(2);
			return;
		}

		atexit(Mix_Quit);
	}

	void Application::Update(std::chrono::microseconds elapsedTime)
	{
		
	}

	void Application::Run()
	{

	}

	void Application::Draw(std::chrono::microseconds elapsedTime)
	{

	}

	void Application::ProcessInput(std::chrono::microseconds elapsedTime)
	{
		
	}

	void Application::LoadContent()
	{

	}

	void Application::ChangeScreens()
	{

	}

	void Application::AddNewEntities()
	{

	}

	void Application::RemoveOldEntities()
	{

	}

}
