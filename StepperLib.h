/*
 Name:		StepperLib.h
 Created:	11/21/2017 4:01:04 PM
 Author:	snekw
 Editor:	http://www.visualmicro.com
*/

#ifndef _StepperLib_h
#define _StepperLib_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif
class Stepper
{
public:
	Stepper(uint8_t steps, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4);
	void setSpeed(uint8_t rpm);
	void step(int steps);
	void stop() const;
	void hold() const;
	enum StepMode
	{
		fullStep = 16,
		halfStep = 8,
		quaterStep = 4,
		eightStep = 2,
		sixteenthStep = 1
	};
	void setStepMode(StepMode mode);
private:
	uint8_t pin1_;
	uint8_t pin2_;
	uint8_t pin3_;
	uint8_t pin4_;
	unsigned long stepDelay_ = 0;
	uint8_t steps_;
	int8_t currentStep_ = 0;
	uint8_t direction_ = 0;
	StepMode stepMode_ = fullStep;

	static const int stepTable_[2][64];

	void stepperStep(uint8_t stepNumber) const;
};

#endif

