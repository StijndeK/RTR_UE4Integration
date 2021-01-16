#include "Timer.h"

void Timer::setLength(float timeInMs)
{
	addValue = (1.0 / 60) * (1 / (timeInMs / 1000.0));
}

float Timer::timerTick()
{
	if (currentTick < 1) {
		currentTick += addValue;
		return 0.0f;
	}
	else {
		currentTick = 0;
		return 1.f;
	}
}
