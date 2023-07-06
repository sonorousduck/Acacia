#include "SoundDevice.hpp"
#include <AL/al.h>
#include <iostream>

static SoundDevice* _instance = nullptr;


SoundDevice* SoundDevice::get()
{
	init();
	return _instance;
}

void SoundDevice::init()
{
	if (_instance == nullptr)
		_instance = new SoundDevice();
}


SoundDevice::SoundDevice()
{
	m_ALCDevice = alcOpenDevice(nullptr);

	if (!m_ALCDevice)
	{
		throw("Failed to get sound device");
	}

	m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);
	if (!m_ALCContext)
	{
		throw("Failed to set sound context");
	}

	if (!alcMakeContextCurrent(m_ALCContext))
	{
		throw("Failed to make context current");
	}

	const ALCchar* name = nullptr;
	if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
	{
		name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
	}
	if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
	{
		name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
	}
	std::cout << "Opened: " << name << std::endl;

}

SoundDevice::~SoundDevice()
{
	if (!alcMakeContextCurrent(nullptr))
	{
		throw("Failed to set context to nullptr");
	}

	alcDestroyContext(m_ALCContext);
	if (m_ALCContext)
	{
		throw("Failed to unset during close");
	}

	if (!alcCloseDevice(m_ALCDevice))
	{
		throw("Failed to close sound device");
	}

}