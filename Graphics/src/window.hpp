#pragma once
#include "glad/glad.h"
#include <SDL.h>
#include <SDL_mixer.h>

namespace Ebony 
{
	class Window
	{
	public:
		Window() {};
		void createWindow(int versionMajor, int versionMinor, int screenWidth, int screenHeight);
		void createWindow(const char* windowName, int versionMajor, int versionMinor, int screenWidth, int screenHeight);

		SDL_Window* getWindow();

	private:
		/*GLFWwindow* window{};*/
		SDL_Window* window{};
		SDL_GLContext mainContext;
	};

}