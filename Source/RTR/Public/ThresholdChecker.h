#pragma once
#include "CoreMinimal.h"

#pragma once
#include "CoreMinimal.h"

class ThresholdChecker
{
public:
	void setFunctionToCall(void(&functionToCall)());
	void (*_functionToCall)();
};

class ActionCalculator
{
public:
	// base treshold checker functions
	void setThreshold(float timeInMs);

	// own functions
	void update(float currentValue);
	void startActionCalculator();
	void stopActionCalculator();
	bool calculateAction = false;

	void setFunctionsToCall(void(&triggerFunctionToCall)(), void(&setFunctionToCall)(float pos));

private:
	std::vector<float> values;
	std::vector<float> values2;
	int currentVecIt = 0;
	float threshold = 0;
	int sampleSize = 200;
	int updateRate = 50;
	bool active;
	bool currentActive;

	void (*_triggerFunctionToCall)();
	void (*_setFunctionToCall)(float pos);
};

class Timer : public ThresholdChecker
{
public:
	void setLength(float timeInMs);
	void update();
	void startTimer();
	void stopTimer();

	bool looping = false;
private:
	float currentTick = 0;
	float addValue;
};

