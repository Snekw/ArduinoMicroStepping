/*
 Name:		StepperLib.cpp
 Created:	11/21/2017 4:01:04 PM
 Author:	snekw
 Editor:	http://www.visualmicro.com
*/

#include "StepperLib.h"

const int Stepper::stepTable_[2][64] = {
	{ 0,25,50,74,98,120,142,162,180,197,212,225,236,244,250,254,255,254,250,244,236,225,212,197,180,162,142,120,98,74,50,25,0,-25,-50,-74,-98,-120,-142,-162,-180,-197,-212,-225,-236,-244,-250,-254,-255,-254,-250,-244,-236,-225,-212,-197,-180,-162,-142,-120,-98,-74,-50,-25 },
	{ 255,254,250,244,236,225,212,197,180,162,142,120,98,74,50,25,0,-25,-50,-74,-98,-120,-142,-162,-180,-197,-212,-225,-236,-244,-250,-254,-255,-254,-250,-244,-236,-225,-212,-197,-180,-162,-142,-120,-98,-74,-50,-25,0,25,50,74,98,120,142,162,180,197,212,225,236,244,250,254 }
};


/**
 * \brief Initialize the stepper
 * \param steps How many steps for full rotation
 * \param pin1 The stepper pin 1
 * \param pin2 The stepper pin 2
 * \param pin3 The stepper pin 3
 * \param pin4 The stepper pin 4
 */
Stepper::Stepper(uint8_t steps, uint8_t pin1, uint8_t pin2, uint8_t pin3, uint8_t pin4)
	:
	pin1_(pin1),
	pin2_(pin2),
	pin3_(pin3),
	pin4_(pin4),
	steps_(steps)
{
	pinMode(pin1_, OUTPUT);
	pinMode(pin2_, OUTPUT);
	pinMode(pin3_, OUTPUT);
	pinMode(pin4_, OUTPUT);
}

/**
 * \brief Set the speed of the motor
 * \param rpm Revolutions per second
 */
void Stepper::setSpeed(uint8_t rpm)
{
	stepDelay_ = 60L * 1000L / steps_ / rpm;
}

/**
 * \brief Make n many steps.
 * \param steps How many steps.
 */
void Stepper::step(int steps)
{
	direction_ = (steps > 0) ? 1 : -1;
	unsigned long stepsLeft = abs(steps) * (16L/stepMode_);

	while (stepsLeft > 0)
	{
		/*Serial.print(currentStep_);
		Serial.print(' ');*/
		stepperStep(currentStep_);
		if (direction_ > 0)
		{
			currentStep_ += stepMode_;
			if(currentStep_ > 63)
			{
				currentStep_ = currentStep_ - 64;
			}
		}
		else
		{
			currentStep_ -= stepMode_;
			if (currentStep_ < 0)
			{
				currentStep_ = 64 + currentStep_;
			}
		}
		stepsLeft--;
	}
}


/**
 * \brief Put the stepper in rest mode. Can cause small rotation!
 */
void Stepper::stop() const
{
	digitalWrite(pin1_, 0);
	digitalWrite(pin2_, 0);
	digitalWrite(pin3_, 0);
	digitalWrite(pin4_, 0);
}


/**
 * \brief Put the stepper in hold mode. Can cause small rotation!
 */
void Stepper::hold() const
{
	digitalWrite(pin1_, 1);
	digitalWrite(pin2_, 0);
	digitalWrite(pin3_, 1);
	digitalWrite(pin4_, 0);
}

/**
 * \brief Set the step mode of the stepper.
 * \param mode Stepping mode
 */
void Stepper::setStepMode(StepMode mode)
{
	stepMode_ = mode;
}

/**
 * \brief Take a step.
 * \param stepNumber The stepnumber.
 */
void Stepper::stepperStep(uint8_t stepNumber) const
{
	int step = stepTable_[0][stepNumber];
	int step2 = stepTable_[1][stepNumber];
	//Serial.print(step);
	//Serial.print(' ');
	//Serial.println(step2);
	if (step > 0)
	{
		digitalWrite(pin1_, 0);
		analogWrite(pin2_, abs(step));
	}
	else
	{
		digitalWrite(pin2_, 0);
		analogWrite(pin1_, abs(step));
	}
	if (step2 > 0)
	{
		digitalWrite(pin3_, 0);
		analogWrite(pin4_, abs(step2));
	}
	else
	{
		digitalWrite(pin4_, 0);
		analogWrite(pin3_, abs(step2));
	}
	delay(stepDelay_ * stepMode_/16);
}
