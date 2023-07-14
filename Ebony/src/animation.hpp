#pragma once

#include "spritesheet.hpp"
#include <functional>

class Animation
{
	SpriteSheet spritesheet;
	int currentFrame;
	float currentTime;
	bool shouldRepeat;
	bool shouldRepeatForever;
	int repeatTimes;
	bool started = false;

	std::function<void*> onAnimationStart;
	std::function<void*> onAnimationUpdate;
	std::function<void*> onAnimationEnd;
};