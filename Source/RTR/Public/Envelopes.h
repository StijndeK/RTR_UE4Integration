#pragma once

class Envelopes
{
public:
	double arLin(double input, int trigger);
	double arExp(double input, int trigger);
	double arAttackExp(double input, int trigger);

	void setARLin(double attackMs, double releaseMs);
	void setARExp(double attackMs, double releaseMs);
	void setAExp(double attackMs);

	// AR envelope states and initial state.
	enum ArLinState { ATTACK, HOLD, RELEASE, STOP };
	ArLinState currentEnvState = STOP;

private:
	double output;
	double amplitude = 0.001;
	double amplitudeExp, amplitudeLin;
	double amplitudeStartValue = 0.01;

	double updateRate = 60.0; // oF update() rate

	float attack = 0;
	float release = 0;
};

