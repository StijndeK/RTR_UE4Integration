#include "Modulation.h"
using namespace std;

void ModulationBase::CalculateAttackDecreaseStepSize(float attackDownSec) {
	if (modType == linear) {
		downStep = 0 - ((1.0 / updateRate) * (1.0 / attackDownSec));
	}
	else {
		downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackDownSec));

		downStepExp = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackDownSec));
		downStepAc = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackDownSec));
	}
}

//--------------------------------------------------------------
// updown Modulation Base
//--------------------------------------------------------------

void UpDownModulationBase::CalculateAttackStepSize(float attackUpSec) {
	if (modType == linear) {
		upStep = (1.0 / updateRate) * (1.0 / attackUpSec);
	}
	else {
		upStep = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackUpSec));

		upStepExp = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * attackUpSec));
		upStepAc = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * attackUpSec));
	}
}

//--------------------------------------------------------------
// Position Modulation
//--------------------------------------------------------------

PositionModulation::PositionModulation()
{
	currentDistance = amplitudeStartValue;
	currentDistanceExp = amplitudeStartValue;
	currentDistanceAc = 1;
}

float PositionModulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger) {
	// attack stage
	if (trigger == 1) {
		float buffer = 0.01; // buffer because value might not get to exact goal because of step sizes
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer) {
			if (modType == linear) {
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else {
				if (currentDistance < amplitudeStartValue) currentDistance = amplitudeStartValue;
				if (currentDistanceExp < amplitudeStartValue) currentDistanceExp = amplitudeStartValue;
				if (currentDistanceAc < amplitudeStartValue) currentDistanceAc = amplitudeStartValue;

				if (currentDistance < currentDistanceToGetToInRange) {
					currentDistanceExp *= upStepExp;
					currentDistanceAc *= upStepAc;
				}
				else {
					currentDistanceExp *= downStepExp;
					currentDistanceAc *= downStepAc;
				}

				//currentDistance *= (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;

				// set curve based on ratio between Ac and Exp
				currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));

				//cout << "test" << test << endl;
				cout << "currentDistance" << currentDistance << endl;
			}
		}
	}

	// release value is fixed, exponential, and not influenced by game data
	else {
		if (currentDistance > 0.01) {
			currentDistance *= release;

			// reset attack values
			if (currentDistanceExp != amplitudeStartValue) currentDistanceExp = amplitudeStartValue;
			if (currentDistanceAc != 1) currentDistanceAc = 1;
		}
	}

	return currentDistance;
}

void PositionModulation::CalculateReleaseStepSize(float releaseSec) {
	release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * releaseSec));
}

//--------------------------------------------------------------
// Time Modulation
//--------------------------------------------------------------

TimeModulation::TimeModulation()
{
	curveRatio = 0.3;
	currentDistance = 1;
	currentDistanceExp = 1;
	currentDistanceAc = amplitudeStartValue;
}

// reduce intensity when trigger = 1
float TimeModulation::CalculateModulation(int trigger)
{
	if (trigger == 1) {
		if (currentDistance > range) {
			// calculate exp curve
			currentDistanceExp *= downStepExp;

			// calculate AC curve
			currentDistanceAc *= downStepAc;

			// set curve based on ratio between Ac and Exp
			currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));
		}
	}

	else {
		currentDistance = 1;
		currentDistanceExp = 1;
		currentDistanceAc = amplitudeStartValue;
	}

	return currentDistance;
}

//--------------------------------------------------------------
// Action Modulation
//--------------------------------------------------------------

ActionModulation::ActionModulation()
{
	currentDistance = 1;
	currentDistanceExp = 1;
	currentDistanceAc = amplitudeStartValue;
}

float ActionModulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger) {
	if (trigger == 1) {
		float buffer = 0.01;
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer && currentDistance >= range) {
			if (modType == linear) {
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else {
				if (currentDistance < amplitudeStartValue) currentDistance = amplitudeStartValue;

				if (currentDistance < currentDistanceToGetToInRange) {
					currentDistanceExp *= upStepExp;
					currentDistanceAc *= upStepAc;
				}
				else {
					currentDistanceExp *= downStepExp;
					currentDistanceAc *= downStepAc;
				}

				//currentDistance *= (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;

				// set curve based on ratio between Ac and Exp
				currentDistance = ((1 - currentDistanceAc) * curveRatio) + (currentDistanceExp * (1 - curveRatio));
			}
		}
	}


	else {
		currentDistance = 1;
		currentDistanceExp = 1;
		currentDistanceAc = amplitudeStartValue;
	}

	return currentDistance;
}
