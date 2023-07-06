#pragma once
#include <AL/alc.h>

class SoundDevice
{
public:
	static SoundDevice* get();
	static void init();
private:
	SoundDevice();
	~SoundDevice();

	ALCdevice* m_ALCDevice;
	ALCcontext* m_ALCContext;
};