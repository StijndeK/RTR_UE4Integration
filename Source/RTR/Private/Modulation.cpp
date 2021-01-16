#include "Modulation.h"

float Modulation::CalculateModulation(float currentDistanceToGetToInRange, int trigger) {
	// attack stage
	if (trigger == 1) {
		float buffer = 0.01; // buffer because value might not get to exact goal because of step sizes
		if (currentDistance < currentDistanceToGetToInRange - buffer || currentDistance > currentDistanceToGetToInRange + buffer) {
			if (modType == linear) {
				currentDistance += (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
			else {
				if (currentDistance < amplitudeStartValue) { currentDistance = amplitudeStartValue; }; // make sure multiplyvalue != 0 TODO: do automatically
				currentDistance *= (currentDistance < currentDistanceToGetToInRange) ? upStep : downStep;
			}
		}
	}

	// release / hold at 0 stage
	// release value is fixed and not influenced by game data
	// release modulation is always exponential
	else {
		if (currentDistance > 0.01) {
			currentDistance *= release;
		}
	}

	return currentDistance;
}

void Modulation::CalculateStepSize(float attackUpMs, float attackDownMs, float releaseMs) {
	if (modType == linear) {
		upStep = (1.0 / updateRate) * (1 / (attackUpMs / 1000.0));
		downStep = 0 - ((1.0 / updateRate) * (1 / (attackDownMs / 1000.0)));
	}
	else {
		upStep = pow((1.0 / amplitudeStartValue), 1.0 / (updateRate * (attackUpMs / 1000.0)));
		downStep = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (attackDownMs / 1000.0)));
	}
	release = pow((amplitudeStartValue / 1.0), 1.0 / (updateRate * (releaseMs / 1000.0)));
}
