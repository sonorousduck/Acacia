#pragma once

#include <al.h>
#include <iostream>

class SoundSource
{
public:
	SoundSource();
	
	void Delete();
	void Play(const ALuint buffer);
	bool isPlaying = false;
	ALuint source = 0;

private:
	float pitch = 1.0f;
	float gain = 1.0f;
	float positions[3] = { 0, 0, 0 };
	float velocity[3] = { 0, 0, 0 };
	bool loopSound = false;
	ALuint buffer = 0;
};