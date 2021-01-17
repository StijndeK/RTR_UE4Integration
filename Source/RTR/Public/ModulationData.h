#pragma once

class ModulationData
{
public:
	void MockData();
	float ConvertToDecimalData();

	float currentDistanceToGetTo = 1;
	float currentActionToGetTo = 1;
	float positionGoal;
	float positionStart;
	float totalDistance;
};

