#ifndef LAZER_ORIG
#define LAZER_ORIG

#include "Arduino.h"
#include "Wire.h"
#include "VL53L0X.h"
// #include "advMath.h"

class Lazer {
	public:
		Lazer(uint8_t get_X, double get_MULTI_AVG, uint16_t get_BORDER_IS_CLOSE);
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