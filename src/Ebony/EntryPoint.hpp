#pragma once


#ifdef EB_PLATFORM_WINDOWS

extern Ebony::Application* Ebony::CreateApplication();

int main(int argc, char** argv)
{
	Ebony::Log::Init();

	auto app = Ebony::CreateApplication();
	app->Run();
	delete app;
}

#endif