//#include "SoundDevice.hpp"
//#include <AL/al.h>
//#include <iostream>
//
//static SoundDevice* _instance = nullptr;
//
//
//SoundDevice* SoundDevice::get()
//{
//	init();
//	return _instance;
//}
//
//void SoundDevice::init()
//{
//	try 
//	{
//		if (_instance == nullptr)
//		{
//			_instance = new SoundDevice();
//		}
//	}
//	catch (std::string error)
//	{
//		std::cout << error << std::endl;
//		// TODO: Initialize a false object
//	}
//	
//}
//
//
//SoundDevice::SoundDevice()
//{
//	m_ALCDevice = alcOpenDevice(nullptr);
//
//	if (!m_ALCDevice)
//	{
//		throw("Failed to get sound device");
//	}
//
//	m_ALCContext = alcCreateContext(m_ALCDevice, nullptr);
//	if (!m_ALCContext)
//	{
//		throw("Failed to set sound context");
//	}
//
//
//	if (!alcMakeContextCurrent(m_ALCContext))
//	{
//		throw("Failed to make context current");
//	}
//
//	const ALCchar* name = nullptr;
//	if (alcIsExtensionPresent(m_ALCDevice, "ALC_ENUMERATE_ALL_EXT"))
//	{
//		name = alcGetString(m_ALCDevice, ALC_ALL_DEVICES_SPECIFIER);
//	}
//	if (!name || alcGetError(m_ALCDevice) != AL_NO_ERROR)
//	{
//		name = alcGetString(m_ALCDevice, ALC_DEVICE_SPECIFIER);
//	}
//	std::cout << "Opened: " << name << std::endl;
//
//}
//
//
//// https://stackoverflow.com/questions/58995268/how-to-check-result-of-alcdestroycontext
//SoundDevice::~SoundDevice()
//{
//	ALCboolean check = alcMakeContextCurrent(NULL);
//	if (!check)
//	{
//		std::cerr << "[ERROR]::alcMakeContextCurrent() failed to make current context NULL.\n";
//	}
//
//	alcDestroyContext(m_ALCContext);
//	ALenum err = alcGetError(m_ALCDevice);
//	if (err != AL_NO_ERROR)
//	{
//		std::cerr << "[ERROR]::alcDestroyContext() error: " << alcGetString(m_ALCDevice, err) << std::endl;
//	}
//
//	check = alcCloseDevice(m_ALCDevice);
//	if (!check)
//	{
//		std::cerr << "[ERROR]::alcCloseDevice() failed to close the device.\n";
//	}
//}