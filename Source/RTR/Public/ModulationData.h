#pragma once

class ModulationData
{
public:
	void MockData();
	float ConvertToDecimalData();

	float currentDistanceToGetTo = 0;
	float currentActionToGetTo = 1;
	float positionGoal;
	float positionStart;
	float totalDistance;
};

