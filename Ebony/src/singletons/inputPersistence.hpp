#pragma once
#include "../components/keyboardComponent.hpp"
#include "../components/mouseInputComponent.hpp"
#include "../components/controllerComponent.hpp"





namespace Ebony
{
	class InputPersistence
	{
	public:
		static bool keyboardLoaded;
		static bool mouseLoaded;
		static bool controllerLoaded;

		static bool getControlsLoaded()
		{
			return keyboardLoaded && mouseLoaded && controllerLoaded;
		}

		static void LoadSavedKeyboardControls(components::KeyboardInput& keyboard);
		static void SaveKeyboardControls(components::KeyboardInput& keyboard);

		static void LoadSavedMouseControls(components::MouseInput& mouse);
		static void SaveMouseControls(components::MouseInput& mouse);

		static void LoadSavedControllerControls(components::ControllerInput& controller);
		static void SaveControllerControls(components::ControllerInput& controller);

		static void LoadControls(components::KeyboardInput& keyboard, components::MouseInput& mouse, components::ControllerInput& controller);


	private:

		InputPersistence() 
		{
			keyboardLoaded = false;
			mouseLoaded = false;
			controllerLoaded = false;
		}


		static bool isLoadingKeyboard;
		static bool isLoadingMouse;
		static bool isLoadingController;

		static bool isSavingKeyboard;
		static bool isSavingMouse;
		static bool isSavingController;
	};
}