#pragma once

#ifdef EB_PLATFORM_WINDOWS

extern Ebony::Application* Ebony::CreateApplication();

int main(int argc, char** argv)
{
	auto app = Ebony::CreateApplication();
	app->Run();
	delete app;
}

#endif