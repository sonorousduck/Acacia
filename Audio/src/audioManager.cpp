#include "audioManager.hpp"
#include "EmptySoundDevice.hpp"


Ebony::AudioManager Ebony::AudioManager::Init()
{
	SoundDevice* device = SoundDevice::get();

	return AudioManager(device);
}

SoundSource Ebony::AudioManager::createSoundSourceObject(SoundSource sound, AudioType type)
{
	return SoundSource();
}
