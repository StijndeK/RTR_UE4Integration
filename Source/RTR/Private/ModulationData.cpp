#include "ModulationData.h"

// Get the value to get to as a float between 0 and 1 (to feed into FMOD_setVolume).
float ModulationData::ConvertToDecimalData(float position) 
{
	return 1 - ((position - positionStart) / totalDistance);
}