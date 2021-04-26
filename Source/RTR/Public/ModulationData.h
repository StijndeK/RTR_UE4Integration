#pragma once

class ModulationData
{
public:
	float ConvertToDecimalData(float position);

	float currentDistanceToGetTo = 1;
	float positionGoal;
	float positionStart;
	float totalDistance;
};

