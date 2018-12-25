#ifndef INA219_ORIG
#define INA219_ORIG

#include "Arduino.h"
#include "advMath.h"
#include "Adafruit_INA219.h"

class Ina219 {
	public:
		Ina219(uint8_t get_X, double get_LOW_VOLT, double get_HIGH_VOLT, uint8_t get_MAX_CR, uint8_t get_MAX_CVL);
		bool checkVolt();
		double getValue();
	private:
		double LOW_VOLT;
		double HIGH_VOLT;

		uint8_t MAX_CR;
		uint8_t countRead = 0;

		Adafruit_INA219 ina219;
		double value;

		uint8_t MAX_CVL;
		uint8_t countVoltLow = 0;
};

#endif