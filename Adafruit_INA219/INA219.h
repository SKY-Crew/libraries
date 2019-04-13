#ifndef INA219_ORIG
#define INA219_ORIG

#include <Arduino.h>
#include "AdvMath.h"
#include "Adafruit_INA219.h"

class INA219 {
	public:
		INA219(uint8_t, double, double, uint8_t, uint8_t);
		bool checkVolt();
		double getVal();
	private:
		double LOW_VOLT;
		double HIGH_VOLT;

		uint8_t MAX_CR;
		uint8_t countRead = 0;

		Adafruit_INA219 ina219;
		double val;

		uint8_t MAX_CVL;
		uint8_t countVoltLow = 0;
};

#endif