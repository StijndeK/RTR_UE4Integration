#pragma once
#include "CoreMinimal.h"
#include "ModulationTypes.h"

class ModulationBase : public ModulationTypes
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

	modulationType modType = exponential;

	void CalculateAttackDecreaseStepSize(float attackDownSec);
};

class UpDownModulationBase : public ModulationBase
{
public:
	void CalculateAttackStepSize(float attackUpSec);

	float upStepAc, upStepExp;
	float upStep;
};

class PositionModulation : public UpDownModulationBase
{
public:
	PositionModulation();
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
	void CalculateReleaseStepSize(float releaseSec);

private:
	float release;
};

class TimeModulation : public ModulationBase
{
public:
	TimeModulation();
	float CalculateModulation(int trigger);
};

class ActionModulation : public UpDownModulationBase
{
public:
	ActionModulation();
	float CalculateModulation(float currentDistanceToGetToInRange, int trigger);
};