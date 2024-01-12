#pragma once

//#include "Core.hpp"
//#include "events/Event.hpp"
//#include "events/ApplicationEvent.hpp"
#include <chrono>
//#include <iostream>
//#include <graphics2d.hpp>
//#include "singletons/time.hpp"

namespace Ebony {
	class Application
	{
	public:
		Application();
		virtual ~Application();
		
		virtual void Run();
		/*{
			Init();
			auto firstTime = std::chrono::system_clock::now();
			float currentFrame = static_cast<float>(glfwGetTime());
			auto previousTime = std::chrono::system_clock::now();
			
			while (true)
			{
				auto currentTime = std::chrono::system_clock::now();
				auto elapsedTime = std::chrono::duration_cast<std::chrono::microseconds>(currentTime - previousTime);
				Ebony::Time::SetDeltaTime(elapsedTime);
				previousTime = currentTime;

				ProcessInput(elapsedTime);
				Update(elapsedTime);
				Draw(elapsedTime);

				RemoveOldEntities();
				AddNewEntities();
				glfwPollEvents();
				ChangeScreens();		
			}
		}*/

		virtual void Init();
		virtual void LoadContent();
		virtual void Start();
		virtual void AddNewEntities();
		virtual void RemoveOldEntities();
		virtual void Update(std::chrono::microseconds elapsedTime);
		virtual void Draw(std::chrono::microseconds elapsedTime);
		virtual void ProcessInput(std::chrono::microseconds elapsedTime);
		virtual void ChangeScreens();

		
		//float deltaTime = 0.0f;
		//float lastFrame = 0.0f;

	};

	// To be defined in a client
	Application* CreateApplication();
}