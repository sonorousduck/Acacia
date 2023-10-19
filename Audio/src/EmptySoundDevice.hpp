#pragma once

#include "SoundDevice.hpp"

class EmptySoundDevice : SoundDevice
{
	static EmptySoundDevice* get();
	static void init();

private:
	EmptySoundDevice();
	~EmptySoundDevice();
};