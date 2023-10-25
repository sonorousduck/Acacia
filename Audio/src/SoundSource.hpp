#pragma once

#include <al.h>
#include <iostream>
#include "AudioType.hpp"

class SoundSource
{
public:
	SoundSource(EbonyAudio::AudioType type = EbonyAudio::AudioType::ENTITY);
	
	void Delete();
	void Play(const ALuint buffer);
	bool isPlaying = false;
	ALuint source = 0;
	EbonyAudio::AudioType type;

private:
	float pitch = 1.0f;
	float gain = 1.0f;
	float positions[3] = { 0, 0, 0 };
	float velocity[3] = { 0, 0, 0 };
	bool loopSound = false;
	ALuint buffer = 0;
};