#include "inputPersistence.hpp"
#include "rapidjson/prettywriter.h"
#include "rapidjson/rapidjson.h"
#include "rapidjson/stringbuffer.h"
#include "rapidjson/writer.h"
#include "rapidjson/document.h"

void Ebony::InputPersistence::LoadSavedKeyboardControls(components::KeyboardInput& keyboard)
{


}

void Ebony::InputPersistence::SaveKeyboardControls(components::KeyboardInput& keyboard)
{


}

void Ebony::InputPersistence::LoadSavedMouseControls(components::MouseInput& mouse)
{


}

void Ebony::InputPersistence::SaveMouseControls(components::MouseInput& mouse)
{


}

void Ebony::InputPersistence::LoadSavedControllerControls(components::ControllerInput& controller)
{


}

void Ebony::InputPersistence::SaveControllerControls(components::ControllerInput& controller)
{


}

void Ebony::InputPersistence::LoadControls(components::KeyboardInput& keyboard, components::MouseInput& mouse, components::ControllerInput& controller)
{
	LoadSavedKeyboardControls(keyboard);
	LoadSavedControllerControls(controller);
	LoadSavedMouseControls(mouse);
}
