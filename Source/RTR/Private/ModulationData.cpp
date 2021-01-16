#include "ModulationData.h"

// set values that in the engine version need to be set using BPs
void ModulationData::MockData() {
	positionGoal = 10000;
	positionStart = 2000;
	totalDistance = positionGoal - positionStart;
}

// get the value to get to as a float between 0 and 1 (to feed into FMOD_setVolume)
float ModulationData::ConvertToDecimalData() {
	return 1 - ((currentDistanceToGetTo - positionStart) / totalDistance);
}