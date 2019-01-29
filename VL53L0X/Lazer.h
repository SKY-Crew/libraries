#ifndef LAZER_ORIG
#define LAZER_ORIG

#include <Arduino.h>
#include "Wire.h"
#include "VL53L0X.h"
#include "AdvMath.h"

class Lazer {
	public:
		Lazer(uint8_t given_X, double given_MULTI_AVG, uint16_t given_BORDER_IS_CLOSE);
		bool get();
		double getValue();
	private:
		double MULTI_AVG;
		uint16_t BORDER_IS_CLOSE;

		VL53L0X *lazer;

		double value;
		double prv = 0;

};

#endif