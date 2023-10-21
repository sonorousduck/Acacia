#include "audioManager.hpp"
#include "EmptySoundDevice.hpp"


// NOTE: It looks like 256 is a good amount of sources to allow playing at one time
// https://github.com/MonoGame/MonoGame/issues/4403

EbonyAudio::AudioManager EbonyAudio::AudioManager::Init()
{
	SoundDevice* device = SoundDevice::get();

	UISourcePool = SourcePool(AudioType::UI, 8);
	EntitySourcePool = SourcePool(AudioType::ENTITY, 128);
	MusicSourcePool = SourcePool(AudioType::MUSIC, 4);


	return AudioManager(device);
}

SoundSource EbonyAudio::AudioManager::createSoundSourceObject(SoundSource sound, AudioType type)
{
	return SoundSource();
}

SoundSource EbonyAudio::AudioManager::GetSource(AudioType type)
{
	return SoundSource();
}

void EbonyAudio::AudioManager::ReturnSource(AudioType type)
{
}

void EbonyAudio::AudioManager::SetVolume(float volume, AudioType type)
{
}

void EbonyAudio::AudioManager::Mute(AudioType type)
{
}

void EbonyAudio::AudioManager::SetPlaylist()
{
}

void EbonyAudio::AudioManager::PlayPlaylist()
{
}

void EbonyAudio::AudioManager::PlaySound()
{
}
