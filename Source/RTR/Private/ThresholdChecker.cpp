#include "ThresholdChecker.h"
#include <numeric>
using namespace std;

void ThresholdChecker::setFunctionToCall(void(&functionToCall)())
{
	_functionToCall = functionToCall;
}

//--------------------------------------------------------------
// Action Threshold
//--------------------------------------------------------------

void ActionCalculator::setThreshold(float timeInMs)
{
	threshold = timeInMs;
}

void ActionCalculator::update(float currentValue)
{
	if (calculateAction) {
		if (values.size() < updateRate || values2.size() < updateRate) {
			if (values.size() < updateRate) {
				values.push_back(currentValue);
			}
			else {
				values2.push_back(currentValue);
			}
		}
		else {
			cout << "check action modulation" << endl;

			// calculate average
			float average = accumulate(values.begin(), values.end(), 0.0) / updateRate;
			float average2 = accumulate(values2.begin(), values2.end(), 0.0) / updateRate;

			// check deviation with current average
			if (average2 >= average - threshold && average2 <= average + threshold && average + average2 != 0) {
				active = true;
			}
			else {
				active = false;
			}

			// trigger function
			if (currentActive != active) {
				cout << "trigger action modulation: " << active << endl;

				// call the function
				(*_setFunctionToCall)(1 - active);

				currentActive = active;
			}

			// clear vectors to restart count 
			values.clear();
			values2.clear();
		}
	}

	// TODO: if this similarity is above threshold. call the function and  return the rating for the distancetogettovalue of the modulator
}

void ActionCalculator::startActionCalculator()
{
	values.clear();
	currentVecIt = 0;
	calculateAction = true;
}

void ActionCalculator::stopActionCalculator()
{
	calculateAction = false;
}

void ActionCalculator::setFunctionsToCall(void(&triggerFunctionToCall)(), void(&setFunctionToCall)(float pos))
{
	_triggerFunctionToCall = triggerFunctionToCall;
	_setFunctionToCall = setFunctionToCall;
}

//--------------------------------------------------------------
// Timer Threshold
//--------------------------------------------------------------

void Timer::setLength(float timeInMs)
{
	addValue = (1.0 / 60) * (1 / timeInMs);
}

// returns 0 untill value is 1
void Timer::update()
{
	if (currentTick != 2) {
		if (currentTick < 1) {
			// update tick;
			currentTick += addValue;
		}
		else {
			// call the function
			(*_functionToCall)();

			// check if looping or timer finished
			if (looping) {
				startTimer();
			}
			else {
				stopTimer();
			}
		}
	}
}

void Timer::startTimer()
{
	currentTick = 0;
}

void Timer::stopTimer()
{
	currentTick = 2;
}