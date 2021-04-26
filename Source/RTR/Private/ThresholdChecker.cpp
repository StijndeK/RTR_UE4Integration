#include "ThresholdChecker.h"
#include <numeric>
using namespace std;

void ThresholdChecker::setFunctionToCall(void(&functionToCall)())
{
	this->_functionToCall = functionToCall;
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
	if (calculateAction)
	{
		// Fill the vectors.
		if (values.size() < updateRate || values2.size() < updateRate)
		{
			if (values.size() < updateRate)
			{
				values.push_back(currentValue);
			}
			else
			{
				values2.push_back(currentValue);
			}
		}
		// Calculate the average and check if the function should be triggered.
		else
		{
			cout << "check action modulation" << endl;

			// Calculate average.
			float average = accumulate(values.begin(), values.end(), 0.0) / updateRate;
			float average2 = accumulate(values2.begin(), values2.end(), 0.0) / updateRate;

			// Check deviation with current average.
			if (average2 >= average - threshold && average2 <= average + threshold && average + average2 != 0)
			{
				active = true;
			}
			else
			{
				active = false;
			}

			// Trigger function.
			if (currentActive != active)
			{
				cout << "trigger action modulation: " << active << endl;
				(*_setFunctionToCall)(1 - active);
				currentActive = active;
			}

			// Clear vectors to restart count .
			values.clear();
			values2.clear();
		}
	}
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
	this->_triggerFunctionToCall = triggerFunctionToCall;
	this->_setFunctionToCall = setFunctionToCall;
}

//--------------------------------------------------------------
// Timer Threshold
//--------------------------------------------------------------

void Timer::setLength(float timeInMs)
{
	addValue = (1.0 / 60) * (1 / timeInMs);
}

void Timer::update()
{
	if (currentTick != 2)
	{
		// Update the tick.
		if (currentTick < 1)
		{
			currentTick += addValue;
		}
		// Call the function and reset or stop the timer
		else
		{
			(*_functionToCall)();

			if (looping)
			{
				startTimer();
			}
			else
			{
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