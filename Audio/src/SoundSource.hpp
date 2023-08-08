#pragma once

#include <al.h>
#include <iostream>

class SoundSource
{
public:
	SoundSource();
	~SoundSource();

	void Play(const ALuint buffer);
	bool isPlaying = false;

private:
	ALuint source;
	float pitch = 1.0f;
	float gain = 1.0f;
	float positions[3] = { 0, 0, 0 };
	float velocity[3] = { 0, 0, 0 };
	bool loopSound = false;
	ALuint buffer = 0;
};