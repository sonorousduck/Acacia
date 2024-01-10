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
		//Py_Finalize();
	}

	void Application::Init()
	{
		EB_TRACE("Initializing Python");
		/*Py_Initialize();
		PyRun_SimpleString("import os");
		PyRun_SimpleString("import sys");
		PyRun_SimpleString("sys.path.append(os.getcwd())");*/



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

	void Application::Start()
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
