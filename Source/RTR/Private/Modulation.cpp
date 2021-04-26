#include "Modulation.h"
using namespace std;

// TODO: calculate should not be capitalised
// TODO: comments

void ModulationBase::calculateAttackDecreaseStepSize(float attackDownSec)
{
	if (curveType == linear)
	{
		downStep = 0 - ((1.0 / updateRate) * (1.0 / attackDownSec));
	}
	else
	{
		downStepExp = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackDownSec));
		downStepAc = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackDownSec));
	}
}

//--------------------------------------------------------------
// updown Modulation Base
//--------------------------------------------------------------

void UpDownModulationBase::calculateAttackStepSize(float attackUpSec)
{
	if (curveType == linear)
	{
		upStep = (1.0 / updateRate) * (1.0 / attackUpSec);
	}
	else
	{
		upStepExp = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackUpSec));
		upStepAc = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackUpSec));
	}
}

//--------------------------------------------------------------
// Position Modulation
//--------------------------------------------------------------

positionModulation::positionModulation()
{
	currentDistance = amplitudeStartValue;
	currentDistanceExp = amplitudeStartValue;
	currentDistanceAc = 1;
}

void positionModulation::calculateReleaseStepSize(float releaseSec)
{
	release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * releaseSec));
}

float positionModulation::calculateModulation(float currentDistanceToGetToInRange, int trigger)
{
	// attack stage
	if (trigger == 1)
	{
		// Calculate distance based on the curve type.
		float buffer = 0.01; // A buffer is used because value might not get to exact goal because of step sizes.
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer)
		{
			if (curveType == linear)
			{
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else
			{
				if (currentDistance < amplitudeStartValue) currentDistance = amplitudeStartValue;
				if (currentDistanceExp < amplitudeStartValue) currentDistanceExp = amplitudeStartValue;
				if (currentDistanceAc < amplitudeStartValue) currentDistanceAc = amplitudeStartValue;

				if (currentDistance < currentDistanceToGetToInRange)
				{
					currentDistanceExp *= upStepExp;
					currentDistanceAc *= upStepAc;
				}
				else
				{
					currentDistanceExp *= downStepExp;
					currentDistanceAc *= downStepAc;
				}

				// set curve based on ratio between Ac and Exp
				currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));
			}
		}
	}

	// release value is fixed, exponential, and not influenced by game data
	else {
		if (currentDistance > 0.01)
		{
			currentDistance *= release;

			// reset attack values
			if (currentDistanceExp != amplitudeStartValue) currentDistanceExp = amplitudeStartValue;
			if (currentDistanceAc != 1) currentDistanceAc = 1;
		}
	}

	return currentDistance;
}

//--------------------------------------------------------------
// Time Modulation
//--------------------------------------------------------------

timeModulation::timeModulation()
{
	curveRatio = 0.3;
	currentDistance = 1;
	currentDistanceExp = 1;
	currentDistanceAc = amplitudeStartValue;
}

// reduce intensity when trigger = 1
float timeModulation::calculateModulation(int trigger)
{
	if (trigger == 1)
	{
		if (currentDistance > range)
		{
			// calculate exp curve
			currentDistanceExp *= downStepExp;

			// calculate AC curve
			currentDistanceAc *= downStepAc;

			// set curve based on ratio between Ac and Exp
			currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));
		}
	}

	else
	{
		currentDistance = 1;
		currentDistanceExp = 1;
		currentDistanceAc = amplitudeStartValue;
	}

	return currentDistance;
}

//--------------------------------------------------------------
// Action Modulation
//--------------------------------------------------------------

actionModulation::actionModulation()
{
	currentDistance = 1;
	currentDistanceExp = 1;
	currentDistanceAc = amplitudeStartValue;
}

float actionModulation::calculateModulation(float currentDistanceToGetToInRange, int trigger)
{
	if (trigger == 1)
	{
		float buffer = 0.01;
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer && currentDistance >= range)
		{
			if (curveType == linear)
			{
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else
			{
				if (currentDistance < amplitudeStartValue) currentDistance = amplitudeStartValue;

				if (currentDistance < currentDistanceToGetToInRange)
				{
					currentDistanceExp *= upStepExp;
					currentDistanceAc *= upStepAc;
				}
				else
				{
					currentDistanceExp *= downStepExp;
					currentDistanceAc *= downStepAc;
				}

				// set curve based on ratio between Ac and Exp
				currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));
			}
		}
	}


	else
	{
		currentDistance = 1;
		currentDistanceExp = 1;
		currentDistanceAc = amplitudeStartValue;
	}

	return currentDistance;
}
