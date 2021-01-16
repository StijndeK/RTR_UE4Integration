#pragma once
#include "CoreMinimal.h"

class Timer
{
public:
	void setLength(float timeInMs);
	float timerTick();

	float currentTick = 0;
	float addValue;
};

