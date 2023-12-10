#include "window.hpp"
#include "graphics2d.hpp"


namespace Ebony
{
	void Window::createWindow(int versionMajor, int versionMinor, int screenWidth, int screenHeight)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			std::cerr << "Couldn't initialize SDL" << std::endl;
			exit(2);
			return;
		}

		//if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0)
		//{
		//	std::cerr << "SDL audio could not initialize! SDL Error: " << Mix_GetError() << std::endl;
		//	exit(2);
		//	return;
		//}

		atexit(SDL_Quit);
		SDL_GL_LoadLibrary(NULL);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, versionMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versionMinor);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// If fullscreen
		// window = SDL_CreateWindow(windowName, EB_CRITICALEB_CRITICAL SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);


		window = SDL_CreateWindow("TEST", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL);

		mainContext = SDL_GL_CreateContext(window);

		if (mainContext == NULL)
		{
			std::cerr << "Failed to create OpenGL Context" << std::endl;
			exit(2);
		}

		std::cout << "OpenGL Loaded" << std::endl;
		gladLoadGLLoader(SDL_GL_GetProcAddress);

		//https://wiki.libsdl.org/SDL2/SDL_GL_SetSwapInterval
		// 0 means immediate update, 1 means vsync, -1 is adaptive vsync
		SDL_GL_SetSwapInterval(0);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		return;
	}

	void Window::createWindow(const char* windowName, int versionMajor, int versionMinor, int screenWidth, int screenHeight)
	{
		if (SDL_Init(SDL_INIT_EVERYTHING) < 0)
		{
			std::cerr << "Couldn't initialize SDL" << std::endl;
			exit(2);
			return;
		}


		
		

		atexit(SDL_Quit);
		SDL_GL_LoadLibrary(NULL);

		SDL_GL_SetAttribute(SDL_GL_ACCELERATED_VISUAL, 1);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, versionMajor);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, versionMinor);

		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

		// If fullscreen
		// window = SDL_CreateWindow(windowName, EB_CRITICALEB_CRITICAL SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 0, 0, SDL_WINDOW_FULLSCREEN_DESKTOP | SDL_WINDOW_OPENGL);


		window = SDL_CreateWindow(windowName, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, screenWidth, screenHeight, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);

		mainContext = SDL_GL_CreateContext(window);

		if (mainContext == NULL)
		{
			std::cerr << "Failed to create OpenGL Context" << std::endl;
			exit(2);
		}

		std::cout << "OpenGL Loaded" << std::endl;
		gladLoadGLLoader(SDL_GL_GetProcAddress);
		
		//https://wiki.libsdl.org/SDL2/SDL_GL_SetSwapInterval
		// 0 means immediate update, 1 means vsync, -1 is adaptive vsync
		SDL_GL_SetSwapInterval(0);

		int w, h;
		SDL_GetWindowSize(window, &w, &h);
		glViewport(0, 0, w, h);

		return;
	}

	SDL_Window* Window::getWindow()
	{
		return window;
	}

}
