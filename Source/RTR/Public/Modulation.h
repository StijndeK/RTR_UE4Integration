#pragma once
#include "CoreMinimal.h"
#include "ModulationTypes.h"

class ModulationBase
{
public:
	float updateRate = 60.f;
	double amplitudeStartValue = 0.001;
	float currentDistance = 0;

	float currentDistanceExp = 0;
	float currentDistanceAc = 1;
	float curveRatio = 0.5;
	float downStepExp, downStepAc;
	float downStep;
	float range = 0.1;

	modulationCurveType curveType = exponential;

	void calculateAttackDecreaseStepSize(float attackDownSec);
};

class UpDownModulationBase : public ModulationBase
{
public:
	void calculateAttackStepSize(float attackUpSec);

	float upStepAc, upStepExp;
	float upStep;
};

class positionModulation : public UpDownModulationBase
{
public:
	positionModulation();
	void calculateReleaseStepSize(float releaseSec);
	float calculateModulation(float currentDistanceToGetToInRange, int trigger);

private:
	float release;
};

class timeModulation : public ModulationBase
{
public:
	timeModulation();
	float calculateModulation(int trigger);
};

class actionModulation : public UpDownModulationBase
{
public:
	actionModulation();
	float calculateModulation(float currentDistanceToGetToInRange, int trigger);
};