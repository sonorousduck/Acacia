#include "Ebony.hpp"

class Sandbox : public Ebony::Application
{
public:
	Sandbox()
	{

	}

	~Sandbox() 
	{

	}
};


Ebony::Application* Ebony::CreateApplication()
{
	return new Sandbox();
}